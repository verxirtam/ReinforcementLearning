//============================================================================
// Name        : ReinforcementLearning.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include "Policy.h"
#include "Episode.h"
#include "SimpleMDP.h"
#include "Random.h"
#include "EpsilonSoftOnPolicyMonteCarlo.h"
#include "Communication/TSVOutputContext.h"
#include "Communication/TSVInputContext.h"
#include "Communication/InputMessage_1_0.h"
#include "Communication/OutputMessage_1_0.h"
#include "Communication/Read/ReadStochasticPolicy.h"
#include "Communication/Read/ReadCurrentPolicy.h"
#include "Communication/Write/WriteSinglePolicy.h"
#include "Communication/Write/WritePolicy.h"
#include "Communication/Write/WriteStochasticPolicy.h"
#include "Communication/Write/WriteCurrentPolicy.h"
#include "EV3LineTracer/EV3LineTracer.h"
#include "EV3LineTracer/Communication/InputConfigFile.h"
#include "EV3LineTracer/Communication/InputConfigFileEV3Linetracer_1_0.h"
#include "EV3LineTracer/Communication/InputEV3LineTracer_1_0.h"
#include "EV3LineTracer/Communication/InputCommandNullCommand.h"
#include "EV3LineTracer/Communication/InputCommandSetMDP.h"
#include "EV3LineTracer/Communication/InputCommandExecEpisode.h"
#include "EV3LineTracer/Communication/InputCommandSetCurrentPolicy.h"
#include "EV3LineTracer/Communication/OutputEV3LineTracer_1_0.h"
#include "EV3LineTracer/Communication/OutputCommandNullCommand.h"
#include "EV3LineTracer/Communication/OutputCommandSetMDP.h"
#include "EV3LineTracer/Communication/OutputCommandExecEpisode.h"
#include "EV3LineTracer/Communication/OutputCommandSetCurrentPolicy.h"
#include "EV3LineTracer/Communication/Read/ReadSingleControl.h"
#include "EV3LineTracer/Communication/Read/ReadSingleState.h"
#include "EV3LineTracer/Communication/Read/ReadStateCount.h"
#include "EV3LineTracer/Communication/Read/ReadCostMax.h"
#include "EV3LineTracer/Communication/Read/ReadStep.h"
#include "EV3LineTracer/Communication/Read/ReadEpisode.h"
#include "EV3LineTracer/Communication/Write/WriteInterval.h"
#include "EV3LineTracer/Communication/Write/WriteCostMax.h"
#include "EV3LineTracer/Communication/Write/WriteStateCount.h"
#include "EV3LineTracer/Communication/Write/WriteSingleState.h"
#include "EV3LineTracer/Communication/Write/WriteState.h"
#include "EV3LineTracer/Communication/Write/WriteSingleControl.h"
#include "EV3LineTracer/Communication/Write/WriteControl.h"

using namespace std;
using namespace RL;

/////////////////////////////////////////////////////////////////////
//RL::Policyのテスト
/////////////////////////////////////////////////////////////////////
//値をパラメータ化したテスト
//のためのクラスの定義
class PolicyTest : public ::testing::Test, public ::testing::WithParamInterface<int>
{
protected:
	void SetUp(){}
	void TearDown(){}
};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(InstantiatePolicyTest,PolicyTest,::testing::Values(1,10,50,100));


//デフォルトコンストラクタのテスト
TEST(Policy,DefaultConstructor)
{
	Policy p0;
	//デフォルトコンストラクタが呼ばれ、
	//StateCountが0のPolicyが初期化されていること
	EXPECT_EQ(0,p0.getStateCount());
}

//コピーコンストラクタのテスト
TEST_P(PolicyTest,CopyConstructor)
{
	//サイズを取得する
	int size=GetParam();

	//コピー元の宣言
	Policy p0(size);
	//コピー先の宣言(コピー元と値が異なるようにする)
	Policy p1(p0);
	//コピー元と先の値が等しいことを確認する
	EXPECT_EQ(p0[0],p1[0]);
	//コピー元の値を変更する
	p0.setPolicy(0,1);
	//コピー先の値が変更されていないこと(コピー元と先で値が異なること)を確認
	//この検証に失敗するとunit testが停止し、ターミナルに下記が出力される
	//*** glibc detected *** /home/daisuke/workspace/ReinforcementLearning/Debug/ReinforcementLearning:
	//double free or corruption (fasttop): 0x08472988 ***
	ASSERT_NE(p0[0],p1[0]);
}
//コピー代入演算子のテスト
TEST_P(PolicyTest,CopyAssignmentOperator)
{
	//サイズを取得する
	int size=GetParam();

	//コピー元の宣言
	Policy p0(size);
	//コピー先の宣言(コピー元と値が異なるようにする)
	Policy p1;
	//コピー代入演算子の実行
	p1=p0;
	//コピー元と先の値が等しいことを確認する
	EXPECT_EQ(p0.getStateCount(),p1.getStateCount());
	EXPECT_EQ(p0[0],p1[0]);
	//コピー元の値を変更する
	p0.setPolicy(0,1);
	//コピー先の値が変更されていないこと(コピー元と先で値が異なること)を確認
	//この検証に失敗するとunit testが停止し、ターミナルに下記が出力される
	//*** glibc detected *** /home/daisuke/workspace/ReinforcementLearning/Debug/ReinforcementLearning:
	//double free or corruption (fasttop): 0x08472988 ***
	ASSERT_NE(p0[0],p1[0]);
}
//等号演算子のテスト(不等号演算子のテストも兼ねる)
TEST_P(PolicyTest,EqualityOperator)
{
	//サイズを取得する
	int size=GetParam();

	//比較対象0
	Policy p0(size);
	//比較対象1
	Policy p1(size*2);
	//等号演算子の実行(StateCountが異なるのでfalseとなる)
	EXPECT_EQ(p0==p1,false);
	EXPECT_EQ(p0!=p1,true);
		//コピー
	p1=p0;
	//等号演算子の実行(trueとなる)
	EXPECT_EQ(p0==p1,true);
	EXPECT_EQ(p0!=p1,false);
	//コピー先の値を変更
	p1.setPolicy(size/2,2);
	//等号演算子の実行(falseとなる)
	EXPECT_EQ(p0==p1,false);
	EXPECT_EQ(p0!=p1,true);

}


TEST_P(PolicyTest,init)
{
	const idx size=GetParam();
	idx i;
	Policy p(size);



	for(i=0;i<size;i++)
	{
		p.setPolicy(i,i);
	}
	//サイズのチェック
	EXPECT_EQ(size,p.getStateCount());
	//setterで設定した値が反映されているかのチェック
	for(i=0;i<size;i++)
	{
		EXPECT_EQ(i,p[i]);
	}
	//添字演算子は例外を返さない
	EXPECT_NO_THROW(p[size]);
	//setterは例外を返す
	EXPECT_THROW(p.setPolicy(size,0),std::out_of_range);
	//setterは例外を返す
	EXPECT_THROW(p.setPolicy(-1,0),std::out_of_range);
	//サイズのチェック
	EXPECT_EQ(size,p.getStateCount());
}
//コンストラクタが例外を例外を投げる契機のテスト
TEST(Policy,init_exception)
{
	//コンストラクタの引数が負の場合(大きな正整数)は
	//確保する領域が足りないため例外bad_allocを投げる
	//EXPECT_THROW(Policy p0(-4),std::bad_alloc);
	//EXPECT_THROW(Policy p0(-3),std::bad_alloc);
	//EXPECT_THROW(Policy p0(-2),std::bad_alloc);
	//EXPECT_THROW(Policy p0(-1),std::bad_alloc);
	EXPECT_NO_THROW(Policy p1(0));
	EXPECT_NO_THROW(Policy p1(1));
	EXPECT_NO_THROW(Policy p1(2));
	EXPECT_NO_THROW(Policy p1(3));
}
//Policyの変更のテスト
//operator[]
//SetPolicy()
TEST_P(PolicyTest,change)
{
	const int size=GetParam();
	int i;
	Policy p(size);
	for(i=0;i<size;i++)
	{
		p.setPolicy(i,i);
	}
	for(i=0;i<size;i++)
	{
		EXPECT_EQ(i,p[i]);
	}
	for(i=0;i<size;i++)
	{
		p.setPolicy(i,2*i);
	}
	for(i=0;i<size;i++)
	{
		EXPECT_EQ(2*i,p[i]);
	}

}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//RL::StochasticPolicyのテスト
/////////////////////////////////////////////////////////////////////

//値をパラメータ化したテスト
//のためのクラスの定義
class StochasticPolicyTest : public ::testing::Test , public ::testing::WithParamInterface<int>
{
protected:
	void SetUp(){}
	void TearDown(){}
};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(InstantiateStochasticPolicyTest,StochasticPolicyTest,::testing::Values(1,10,50,100));




//コンストラクタのテスト
TEST(StochasticPolicy,Constructor0)
{
	const idx statecount=10;
	vector<idx> controlcount(statecount);
	idx i;
	for(i=0;i<statecount;i++)
	{
		controlcount[i]=10*(i+1);
	}
	controlcount[0]=1;
	StochasticPolicy sp(controlcount);
	EXPECT_EQ(statecount,sp.getStateCount());
	for(i=0;i<statecount;i++)
	{
		EXPECT_EQ(controlcount[i],sp.getControlCount(i));
	}
}
//コンストラクタのテスト
TEST_P(StochasticPolicyTest,Constructor1)
{
	const idx size=GetParam();
	StochasticPolicyData spd(size);
	for(idx i=0;i<size;i++)
	{
		vector<real> prob;
		if(i==0)
		{
			prob=vector<real>(1,0.0);
		}
		else
		{
			prob=vector<real>(10*(i+1),0.0);
		}
		prob[0]=1.0;
		spd[i]=RandomIdx(prob);
	}
	EXPECT_NO_THROW(StochasticPolicy sp(spd));

	for(idx i=0;i<size;i++)
	{
		spd[i]=RandomIdx(vector<real>(10*(i+1),1.0));
	}
	EXPECT_THROW(StochasticPolicy sp(spd),std::out_of_range);


}
//コンストラクタのテスト
TEST_P(StochasticPolicyTest,Constructor2)
{
	//MDPをベースにStochasticPolicyを初期化する
	RL::SimpleMDP mdp(10);
	EXPECT_NO_THROW(StochasticPolicy sp1(mdp));
	StochasticPolicy sp(mdp);
	EXPECT_NO_THROW(sp.correct());

}
TEST(StochasticPolicy,init_exception)
{
	EXPECT_THROW(StochasticPolicy p0(vector<idx>(10,(idx)-3)),std::bad_alloc);
	EXPECT_THROW(StochasticPolicy p0(vector<idx>(10,(idx)-2)),std::bad_alloc);
	EXPECT_THROW(StochasticPolicy p0(vector<idx>(10,(idx)-1)),std::bad_alloc);

	EXPECT_THROW(StochasticPolicy p1(vector<idx>(10,0)),std::invalid_argument);
	EXPECT_NO_THROW(StochasticPolicy p1(vector<idx>(10,1)));
	vector<idx> c(10,2);
	c[0]=1;
	EXPECT_NO_THROW(StochasticPolicy p1(c));
	vector<idx> d(10,3);
	d[0]=1;
	EXPECT_NO_THROW(StochasticPolicy p1(d));

}

//デフォルトコンストラクタのテスト
TEST(StochasticPolicy,DefaultConstructor)
{
	StochasticPolicy sp;
	EXPECT_EQ(0,sp.getStateCount());
}
//コピーコンストラクタのテスト
TEST_P(StochasticPolicyTest,CopyConstructor)
{
	const idx statecount=GetParam();
	//statecount==1の時はテストを実行しない
	if(statecount==1)
	{
		return;
	}

	vector<idx> controlcount(statecount);
	idx i;
	for(i=0;i<statecount;i++)
	{
		controlcount[i]=10*(i+1);
	}
	//terminate stateのcontrolcountは1
	controlcount[0]=1;
	//コピー元の宣言
	StochasticPolicy p0(controlcount);
	//コピー先の宣言(コピーコンストラクタの実行)
	StochasticPolicy p1(p0);
	//コピー元と先の値が等しいことを確認する
	EXPECT_EQ(p0,p1);
	//コピー元の値を変更する
	idx m=statecount/2;
	vector<real> prob(p0[m].getValueMax(),0.0);
	prob[0]=0.5;
	prob[1]=0.5;
	p0.setProbability(m,prob);
	EXPECT_NO_THROW(p0.correct());
	//コピー先の値が変更されていないこと(コピー元と先で値が異なること)を確認
	//この検証に失敗するとunit testが停止し、ターミナルに下記が出力される
	//*** glibc detected *** /home/daisuke/workspace/ReinforcementLearning/Debug/ReinforcementLearning:
	//double free or corruption (fasttop): 0x08472988 ***
	ASSERT_NE(p0,p1);
}
//コピー代入演算子のテスト
TEST_P(StochasticPolicyTest,CopyAssignmentOperator)
{
	const idx statecount=GetParam();
	//statecount==1の時はテストを実行しない
	if(statecount==1)
	{
		return;
	}
	vector<idx> controlcount(statecount);
	idx i;
	for(i=0;i<statecount;i++)
	{
		controlcount[i]=10*(i+1);
	}
	//terminate stateのcontrolcountは1
	controlcount[0]=1;
	//コピー元の宣言
	StochasticPolicy p0(controlcount);
	//コピー先の宣言
	StochasticPolicy p1;
	//コピー代入演算子の実行
	p1=p0;
	//コピー元と先の値が等しいことを確認する
	EXPECT_EQ(p0[0],p1[0]);
	//コピー元の値を変更する
	idx m=statecount/2;
	vector<real> prob(p0[m].getValueMax(),0.0);
	prob[0]=0.5;
	prob[1]=0.5;
	p0.setProbability(m,prob);
	EXPECT_NO_THROW(p0.correct());
	//コピー先の値が変更されていないこと(コピー元と先で値が異なること)を確認
	//この検証に失敗するとunit testが停止し、ターミナルに下記が出力される
	//*** glibc detected *** /home/daisuke/workspace/ReinforcementLearning/Debug/ReinforcementLearning:
	//double free or corruption (fasttop): 0x08472988 ***
	ASSERT_NE(p0,p1);
}
//等号演算子のテスト(不等号演算子のテストも兼ねる)
TEST_P(StochasticPolicyTest,EqualityOperator)
{
	const idx statecount=GetParam();
	//statecount==1の時はテストを実行しない
	if(statecount==1)
	{
		return;
	}
	vector<idx> controlcount(statecount);
	idx i;
	for(i=0;i<statecount;i++)
	{
		controlcount[i]=10*(i+1);
	}
	//terminate stateのcontrolcountは1
	controlcount[0]=1;
	//比較対象0
	StochasticPolicy p0(controlcount);
	for(i=0;i<statecount;i++)
	{
		controlcount[i]=20*(i+1);
	}
	//terminate stateのcontrolcountは1
	controlcount[0]=1;
	//比較対象1
	StochasticPolicy p1(controlcount);
	//等号演算子の実行(StateCountが異なるのでfalseとなる)
	EXPECT_EQ(p0==p1,false);
	EXPECT_EQ(p0!=p1,true);
	//コピー
	p1=p0;
	//等号演算子の実行(trueとなる)
	EXPECT_EQ(p0==p1,true);
	EXPECT_EQ(p0!=p1,false);
	//コピー先の値を変更
	idx m=statecount/2;
	vector<real> prob(p0[m].getValueMax(),0.0);
	prob[0]=0.5;
	prob[1]=0.5;
	p1.setProbability(m,prob);
	p1.correct();
	//等号演算子の実行(falseとなる)
	EXPECT_EQ(p0==p1,false);
	EXPECT_EQ(p0!=p1,true);
}
TEST_P(StochasticPolicyTest,SetProbability0)
{
	const idx statecount=GetParam();
	//statecount==1の時はテストを実行しない
	if(statecount==1)
	{
		return;
	}
	vector<idx> controlcount(statecount);
	idx i;
	for(i=0;i<statecount;i++)
	{
		controlcount[i]=10*(i+1);
	}
	//terminate stateのcontrolcountは1
	controlcount[0]=1;

	idx m=statecount/2;


	StochasticPolicy p0(controlcount);
	vector<real> prob(p0[m].getValueMax(),0.0);
	prob[0]=0.5;
	prob[1]=0.5;
	p0.setProbability(m,prob);
	EXPECT_EQ(p0[m].getProbability(0),0.5);
	EXPECT_EQ(p0[m].getProbability(1),0.5);

	prob=vector<real>(p0[m].getValueMax(),0.0);
	prob[0]= 1.0;
	prob[1]=-1.0;
	EXPECT_THROW(p0.setProbability(m,prob),std::invalid_argument);

	prob=vector<real>(p0[m].getValueMax(),0.0);
	prob[0]=1.0;
	prob[1]=1.0;
	prob[2]=1.0;
	prob[3]=1.0;
	p0.setProbability(m,prob);
	p0.correct(m);
	EXPECT_EQ(p0[m].getProbability(0),0.25);
	EXPECT_EQ(p0[m].getProbability(1),0.25);
	EXPECT_EQ(p0[m].getProbability(2),0.25);
	EXPECT_EQ(p0[m].getProbability(3),0.25);
}
TEST_P(StochasticPolicyTest,SetProbability1)
{
	const idx statecount=GetParam();
	//statecount==1の時はテストを実行しない
	if(statecount==1)
	{
		return;
	}
	vector<idx> controlcount(statecount);
	idx i;
	for(i=0;i<statecount;i++)
	{
		controlcount[i]=10*(i+1);
	}
	//terminate stateのcontrolcountは1
	controlcount[0]=1;
	StochasticPolicy p0(controlcount);


	i=statecount/2;
	vector<real> prob(10*(i+1),0.0);
	prob[0]=1.0;
	prob[1]=1.0;
	EXPECT_NO_THROW(p0.setProbability(i,prob));
	EXPECT_EQ(p0[i].getProbability(0),0.5);
	EXPECT_EQ(p0[i].getProbability(1),0.5);


	StochasticPolicy p1(controlcount);

	prob=vector<real>(35,0);
	prob[0]=1.0;
	prob[1]=1.0;
	i=statecount/2;
	EXPECT_THROW(p1.setProbability(i,prob),std::out_of_range);
	EXPECT_EQ(p1[i].getProbability(0),1.0);
	EXPECT_EQ(p1[i].getProbability(1),0.0);
}

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//RL::Stepのテスト
/////////////////////////////////////////////////////////////////////


TEST(StepTest,EqualityOperator)
{
	Step step0;
	step0.state=0;
	step0.control=0;
	step0.cost=0.0;

	Step step1=step0;

	EXPECT_EQ(step0==step1,true);
	EXPECT_EQ(step0!=step1,false);


	step1.state=1;

	EXPECT_EQ(step0==step1,false);
	EXPECT_EQ(step0!=step1,true);

	step1=step0;
	step1.control=1;

	EXPECT_EQ(step0==step1,false);
	EXPECT_EQ(step0!=step1,true);

	step1=step0;
	step1.cost=1.0;

	EXPECT_EQ(step0==step1,false);
	EXPECT_EQ(step0!=step1,true);

}

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//RL::Episodeのテスト
/////////////////////////////////////////////////////////////////////

//値をパラメータ化したテストのためのクラスの定義
class EpisodeTest : public ::testing::Test , public ::testing::WithParamInterface<int>
{
protected:

	EpisodeData Steps;
	idx imax;
	void SetUp()
	{
		Steps.clear();
		imax=GetParam();
		for(idx i=0;i<imax;i++)
		{
			Step s;
			s.state  =i%13;
			s.control=i%5;
			s.cost   =i%103;
			Steps.push_back(s);
		}
	}
	void TearDown(){}
public:
	EpisodeTest():
		Steps(),
		imax(0)
	{
	}


};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(InstantiateEpisodeTest,EpisodeTest,::testing::Values(1,10,50,100));


//コンストラクタのテスト
TEST_P(EpisodeTest,Constructor0)
{
	idx imax=GetParam();
	Episode episode0(Steps);
	EXPECT_EQ(episode0.getStepCount(),imax);
	for(idx i=0;i<episode0.getStepCount();i++)
	{
		EXPECT_EQ(episode0[i].state  ,i% 13);
		EXPECT_EQ(episode0[i].control,i%  5);
		EXPECT_EQ(episode0[i].cost   ,i%103);
	}
}

TEST(EpisodeTest,init_exception)
{
}

//デフォルトコンストラクタのテスト
TEST(EpisodeTest,DefaultConstructor)
{
	Episode episode0;
	EXPECT_EQ(episode0.getStepCount(),0);
}
//コピーコンストラクタのテスト
TEST_P(EpisodeTest,CopyConstructor)
{


	//コピー元の宣言
	Episode episode0(Steps);
	//コピー先の宣言(コピーコンストラクタの実行)
	Episode episode1(episode0);

	//コピー元と先の値が等しいことを確認する
	EXPECT_EQ(episode0,episode1);
	//コピー元の値を変更する
	episode0.setStep(imax/2,14,0,0);
	//コピー先の値が変更されていないこと(コピー元と先で値が異なること)を確認
	//この検証に失敗するとunit testが停止し、ターミナルに下記が出力される
	//*** glibc detected *** /home/daisuke/workspace/ReinforcementLearning/Debug/ReinforcementLearning:
	//double free or corruption (fasttop): 0x08472988 ***
	ASSERT_NE(episode0,episode1);



}
//コピー代入演算子のテスト
TEST_P(EpisodeTest,CopyAssignmentOperator)
{

	//コピー元の宣言
	Episode episode0(Steps);
	//コピー先の宣言
	Episode episode1;
	//コピー代入演算子の実行
	episode1=episode0;
	//コピー元と先の値が等しいことを確認する
	EXPECT_EQ(episode0,episode1);
	//コピー元の値を変更する
	episode0.setStep(imax/2,14,0,0);
	//コピー先の値が変更されていないこと(コピー元と先で値が異なること)を確認
	//この検証に失敗するとunit testが停止し、ターミナルに下記が出力される
	//*** glibc detected *** /home/daisuke/workspace/ReinforcementLearning/Debug/ReinforcementLearning:
	//double free or corruption (fasttop): 0x08472988 ***
	ASSERT_NE(episode0,episode1);
}

//等号演算子のテスト(不等号演算子のテストも兼ねる)
TEST_P(EpisodeTest,EqualityOperator)
{
	Episode episode0(Steps);
	for(idx i=0;i<Steps.size();i++)
	{
		Steps[i].state  =Steps[i].state+1;
		Steps[i].control=Steps[i].control+1;
		Steps[i].cost   =Steps[i].cost+1;
	}
	//比較対象1
	Episode episode1(Steps);
	//等号演算子の実行(StateCountが異なるのでfalseとなる)
	EXPECT_EQ(episode0==episode1,false);
	EXPECT_EQ(episode0!=episode1,true);
	//コピー
	episode1=episode0;
	//等号演算子の実行(trueとなる)
	EXPECT_EQ(episode0==episode1,true);
	EXPECT_EQ(episode0!=episode1,false);
	//コピー先の値を変更
	episode1.setStep(0,0,0,-1.0);
	//等号演算子の実行(falseとなる)
	EXPECT_EQ(episode0==episode1,false);
	EXPECT_EQ(episode0!=episode1,true);
}

//SetStep()のテスト
TEST_P(EpisodeTest,setStep)
{
	//episode[i_mod]の内容をSetStep()で変更して、正しく変更されたかを確認する

	Episode episode0(Steps);
	Step step;
	idx i_mod=imax/2;
	step.state  =Steps[i_mod].state+1;
	step.control=Steps[i_mod].control+1;
	step.cost   =Steps[i_mod].cost+1.0;

	episode0.setStep(i_mod,step);

	EXPECT_EQ(episode0[i_mod],step);

	step.state  =Steps[i_mod].state+1;
	step.control=Steps[i_mod].control+1;
	step.cost   =Steps[i_mod].cost+1.0;

	episode0.setStep(i_mod,step);

	EXPECT_EQ(episode0[i_mod],step);
}

//AddStep()のテスト
TEST_P(EpisodeTest,addStep)
{
	Episode episode0(Steps);
	Step step;

	//引数の設定
	step.state  =500;
	step.control=500;
	step.cost   =500.0;

	//AddStep()の実行
	episode0.addStep(step.state,step.control,step.cost);

	//StepCountが1増加していることを確認
	EXPECT_EQ(episode0.getStepCount(),imax+1);
	//末尾のStepがstepと一致していることを確認
	EXPECT_EQ(episode0[imax],step);

	//Stepの値を変更
	step.state  =1000;
	step.control=1000;
	step.cost   =1000.0;

	//AddStep()の実行
	episode0.addStep(step);

	//StepCountが1増加していることを確認
	EXPECT_EQ(episode0.getStepCount(),imax+2);
	//末尾のStepがstepと一致していることを確認
	EXPECT_EQ(episode0[imax+1],step);

}


/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// RL::SimpleMDPのテスト
/////////////////////////////////////////////////////////////////////
//値をパラメータ化したテストのためのクラスの定義
class SimpleMDPTest : public ::testing::Test , public ::testing::WithParamInterface<int>
{
protected:

	void SetUp()
	{
	}
	void TearDown(){}


};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(InstantiateSimpleMDPTest,SimpleMDPTest,::testing::Values(2,10,20));



TEST(SimpleMDPTest,Constructor0)
{
	SimpleMDP smdp;
	EXPECT_EQ(smdp.getStateCount(),2);
	EXPECT_EQ(smdp.getControlCount(0),1);
	EXPECT_EQ(smdp.getControlCount(1),1);
}

TEST_P(SimpleMDPTest,Constructor1)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	EXPECT_EQ(smdp.getStateCount(),statecount);


	EXPECT_EQ(smdp.getControlCount(0),1);
	for(idx i=1;i<statecount-1;i++)
	{
		EXPECT_EQ(smdp.getControlCount(i),2);
	}
	EXPECT_EQ(smdp.getControlCount(statecount-1),1);

}

TEST_P(SimpleMDPTest,getPolicy)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	Policy p;
	smdp.getPolicy(p);
	EXPECT_EQ(p.getStateCount(),statecount);
}
TEST_P(SimpleMDPTest,getRegularPolicy)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	Policy p;
	smdp.getRegularPolicy(p);
	EXPECT_EQ(p.getStateCount(),statecount);
	for(idx i=0;i<p.getStateCount();i++)
	{
		EXPECT_EQ(p[i],0);
	}
}

TEST_P(SimpleMDPTest,getDiscountRate)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	EXPECT_EQ(smdp.getDiscountRate(),1.0);
}

TEST_P(SimpleMDPTest,getNextStep)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	//EXPECT_NE(smdp.GetEpisode().GetStepCount(),0);
	Policy p;
	smdp.getRegularPolicy(p);
	Step s;
	const idx trycount=50000;
	//state毎に推移確率を推定して実際の確率を近似していることを確認する
	for(idx i=0;i<statecount;i++)
	{
		//推移した回数をカウントするvector
		vector<idx> stat(statecount,0);
		s.state=i;
		s.control=p[i];
		s.cost=1.0;
		//試行を行いstate毎に推移した回数をカウントする
		for(idx t=0;t<trycount;t++)
		{
			Step next=smdp.getNextStep(s);
			stat[next.state]++;
		}
		//推定値が実際の確率とほぼ一致(誤差0.01以下)となることを確認する
		if(i==0)
		{
			//state 0 の場合
			EXPECT_EQ(stat[0],trycount);
			for(idx j=1;j<statecount;j++)
			{
				EXPECT_EQ(stat[j],0);
			}
		}
		else if(i==statecount-1)
		{
			//state statecount-1 (末尾)の場合
			for(idx j=0;j<statecount-2;j++)
			{
				EXPECT_EQ(stat[j],0);
			}
			EXPECT_EQ(stat[statecount-2],trycount);
			EXPECT_EQ(stat[statecount-1],0);
		}
		else
		{
			//上記以外の場合
			//terminate stateに向かう確率     :0.75
			//terminate stateから遠ざかる確率:0.25
			for(idx j=0;j<statecount;j++)
			{
				if(j==i-1)
				{
					EXPECT_NEAR(((double)stat[j])/((double)trycount),0.75,0.01);
				}
				else if(j==i+1)
				{
					EXPECT_NEAR(((double)stat[j])/((double)trycount),0.25,0.01);
				}
				else
				{
					EXPECT_EQ(stat[j],0);
				}
			}
		}
	}
}


TEST_P(SimpleMDPTest,GetEpisode0)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);

	//S[i][j]:state i の後state jに遷移した回数
	vector<vector<idx> > S(statecount,vector<idx>(statecount,0));
	//N[i]stateiを訪れた回数
	vector<idx> N(statecount,0);
	//エピソードの取得を繰り返す
	for(idx i=0;i<10000;i++)
	{
		Episode e;
		smdp.getEpisode(e);
		idx stepcount=e.getStepCount();
		//S[][], N[]に集計する
		for(idx n=0;n<stepcount-1;n++)
		{
			S[e[n].state][e[n+1].state]++;
			N[e[n].state]++;
		}
	}

	//集計した結果を表示する
	for(idx n=0;n<statecount;n++)
	{
		for(idx m=0;m<statecount;m++)
		{
			if(N[n]!=0)
			{
				real stat;
				stat=((real)S[n][m])/((real)N[n]);
				//cout<<stat<<",";
				real val=0.0;
				if(n==statecount-1)
				{
					if(m==n-1)
					{
						val=1.0;
					}
				}
				else
				{
					if(m==n-1)
					{
						val=0.75;
					}
					else if(m==n+1)
					{
						val=0.25;
					}
				}
				EXPECT_NEAR(stat,val,0.1);
			}
		}
	}

}

TEST_P(SimpleMDPTest,GetEpisode1)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);

	//policyをランダムに設定する
	Random::Init();
	Policy p;
	StochasticPolicy sp;
	smdp.getStochasticPolicy(smdp.getRegularPolicy(p),sp);
	for(idx i=0;i<sp.getStateCount();i++)
	{
		vector<real> prob(sp.getControlCount(i));
		for(idx u=0;u<sp.getControlCount(i);u++)
		{
			prob[u]=1.0+Random::uniformReal();
		}
		sp.setProbability(i,prob);
	}
	smdp.setCurrentPolicy(sp);
	//S[i][j]:state i の後state jに遷移した回数
	vector<vector<idx> > S(statecount,vector<idx>(statecount,0));
	//N[i]stateiを訪れた回数
	vector<idx> N(statecount,0);
	//エピソードの取得を繰り返す
	for(idx i=0;i<10000;i++)
	{
		Episode e;
		smdp.getEpisode(e);
		idx stepcount=e.getStepCount();
		//S[][], N[]に集計する
		for(idx n=0;n<stepcount-1;n++)
		{
			S[e[n].state][e[n+1].state]++;
			N[e[n].state]++;
		}
	}

	//集計した結果を表示する
	for(idx i=0;i<statecount;i++)
	{
		real val0=sp[i].getProbability(0)*0.75
				+sp[i].getProbability(1)*0.25;
		real val1=sp[i].getProbability(0)*0.25
				+sp[i].getProbability(1)*0.75;
		for(idx j=0;j<statecount;j++)
		{
			if(N[i]!=0)
			{
				real stat;
				stat=((real)S[i][j])/((real)N[i]);
				//cout<<stat<<",";
				real val=0.0;
				if(i==statecount-1)
				{
					if(j==i-1)
					{
						val=1.0;
					}
				}
				else
				{
					if(j==i-1)
					{
						val=val0;
					}
					else if(j==i+1)
					{
						val=val1;
					}
				}
				EXPECT_NEAR(stat,val,0.05);
			}
		}
	}

}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// RL::Randomのテスト
/////////////////////////////////////////////////////////////////////
//値をパラメータ化したテストのためのクラスの定義
class RandomTest : public ::testing::Test , public ::testing::WithParamInterface<int>
{
protected:

	void SetUp()
	{
	}
	void TearDown(){}


};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(InstantiateRandomTest,RandomTest,::testing::Values(2,10,50,100));

TEST_P(RandomTest,uniformIdx)
{
	idx max=GetParam();
	vector<idx> stat(max);
	const idx testcount=10000;

	Random::Init();

	for(idx n=0;n<testcount;n++)
	{
		idx i=Random::uniformIdx(max);
		stat[i]++;
	}
	for(idx i=0;i<max;i++)
	{
		EXPECT_NEAR((real)stat[i]/(real)testcount,1.0/(double)max,0.01);
	}
}

TEST_P(RandomTest,uniformReal)
{
	idx max=GetParam();
	vector<idx> stat(max);
	const idx testcount=100000;

	Random::Init();

	for(idx n=0;n<testcount;n++)
	{
		real r=max*Random::uniformReal();
		idx i=(int)r;
		stat[i]++;
	}
	for(idx i=0;i<max;i++)
	{
		EXPECT_NEAR((real)stat[i]/(real)testcount,1.0/(double)max,0.01);
	}
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// RL::RandomIdxのテスト
/////////////////////////////////////////////////////////////////////
//値をパラメータ化したテストのためのクラスの定義
class RandomIdxTest : public ::testing::Test , public ::testing::WithParamInterface<int>
{
protected:

	void SetUp()
	{
	}
	void TearDown(){}


};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(InstantiateRandomIdxTest,RandomIdxTest,::testing::Values(1,2,10,50,100));

TEST(RandomIdxTest,Constructor)
{
	EXPECT_NO_THROW(RandomIdx a());
	EXPECT_NO_THROW(RandomIdx a(vector<real>(1,1.0),16));
	EXPECT_THROW(RandomIdx a(vector<real>(1,0.0),16),std::invalid_argument);
	EXPECT_THROW(RandomIdx a(vector<real>(1,1.0),0),std::invalid_argument);
	EXPECT_NO_THROW(RandomIdx a(vector<real>(1,1.0),1));

	for(idx i=0;i<100;i++)
	{
		vector<real> prob(10*(i+1),0.0);
		prob[5*(i+1)]=-1.0;
		EXPECT_THROW(RandomIdx a(prob),std::invalid_argument);
	}

}

TEST_P(RandomIdxTest,StatTest)
{
	const idx max=GetParam();
	vector<real> prob(max);
	vector<idx> stat(max,0);
	Random::Init();
	real sum=0.0;
	for(idx i=0;i<max;i++)
	{
		real d=Random::uniformReal();
		prob[i]=d;
		sum+=d;
	}
	for(idx i=0;i<max;i++)
	{
		prob[i]/=sum;
	}


	RandomIdx ri(prob,32);


	const idx testcount=10000;

	for(idx n=0;n<testcount;n++)
	{
		idx i=ri.get();
		stat[i]++;
	}
	for(idx i=0;i<max;i++)
	{
		//cout<<"stat["<<i<<"]="<<stat[i]<<endl;
		EXPECT_NEAR((real)stat[i]/(real)testcount,prob[i],0.01);
	}
}


TEST_P(RandomIdxTest,GetProbabilityTest)
{
	const idx max=GetParam();
	vector<real> prob(max);
	int part_count=16;
	for(idx i=0;i<max;i++)
	{
		prob[i]=(real)(i+1);
	}
	real sum=0.0;
	for(idx i=0;i<prob.size();i++)
	{
		sum+=prob[i];
	}
	for(idx i=0;i<prob.size();i++)
	{
		prob[i]/=sum;
	}

	RandomIdx ri(prob, part_count);

	EXPECT_EQ(ri.getValueMax(),max);
	for(idx i=0;i<prob.size();i++)
	{
		EXPECT_NEAR(ri.getProbability(i),prob[i],0.125*0.125*0.125*0.125);
	}
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// RL::EpsilonSoftOnPolicyMonteCarlo
/////////////////////////////////////////////////////////////////////
//値をパラメータ化したテストのためのクラスの定義
class EpsilonSoftOnPolicyMonteCarloTest : public ::testing::Test , public ::testing::WithParamInterface<int>
{
protected:

	void SetUp()
	{
	}
	void TearDown(){}


};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(InstantiateEpsilonSoftOnPolicyMonteCarloTest,EpsilonSoftOnPolicyMonteCarloTest,::testing::Values(3,10,50,100));


TEST_P(EpsilonSoftOnPolicyMonteCarloTest,PolicyIteration)
{
	const idx statecount=GetParam();//10;//
	SimpleMDP smdp(statecount);
	EpsilonSoftOnPolicyMonteCarlo<SimpleMDP> mc(smdp);
	//optimal policyを取得
	Policy p;
	mc.policyIteration(p,10,100);
	//value function
	vector<vector<real> > Q;
	smdp.setCurrentPolicy(p);
	smdp.getControlValueFunction(Q);

	//optimal policy と異なるポリシーの価値関数が最適とならないことを確認する。
	Policy p1=p;
	vector<vector<real> > Q1;
	Random::Init();
	idx state=Random::uniformIdx(p1.getStateCount()-2)+1;
	idx control=1;//Random::UniformIdx(2);
	p1.setPolicy(state,control);
	smdp.setCurrentPolicy(p1);
	smdp.getControlValueFunction(Q1);

	//価値観数の表示
	//表示しない場合はif文をfalseにする
	if(false)
	{
		cout<<"state="<<state<<endl;
		for(idx i=0;i<Q.size();i++)
		{
			cout<<i<<": ";
			for(idx u=0;u<Q[i].size();u++)
			{
				EXPECT_EQ(Q[i][u]<=Q1[i][u],true);
				cout<<Q[i][u]<<", "<<Q1[i][u]<<" | ";
			}
			cout<<endl;
		}
	}
}

TEST_P(EpsilonSoftOnPolicyMonteCarloTest,EpsilonGreedy)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	EpsilonSoftOnPolicyMonteCarlo<SimpleMDP> mc(smdp);
	Policy rp;
	smdp.getRegularPolicy(rp);
	real e=0.5;
	for(idx k=0;k<10;k++)
	{
		StochasticPolicy sp;
		mc.epsilonGreedy(rp,e,sp);
		for(idx i=0;i<sp.getStateCount();i++)
		{
			idx controlcount=sp.getControlCount(i);
			for(idx u;u<controlcount;u++)
			{
				if(u==rp[i])
				{
					real prob=1.0 - e + e/((real)controlcount);
					EXPECT_NEAR(sp[i].getProbability(u),prob,0.01);
				}
				else
				{
					real prob=e/((real)controlcount);
					EXPECT_NEAR(sp[i].getProbability(u),prob,0.01);
				}
			}
		}

		e*=0.5;
	}
}

TEST_P(EpsilonSoftOnPolicyMonteCarloTest,PolicyEvaluation)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	EpsilonSoftOnPolicyMonteCarlo<SimpleMDP> mc(smdp);
	real e=0.0625*0.5;
	Policy p;
	StochasticPolicy sp;
	mc.epsilonGreedy(smdp.getRegularPolicy(p),e,sp);
	mc.setCurrentPolicy(sp);
	vector<vector<real> > value;

	mc.policyEvaluation(value,10000);
	vector<vector<real> > Q;
	smdp.getControlValueFunction(Q);

	for(idx i=0;i<sp.getStateCount();i++)
	{
		for(idx u=0;u<sp.getControlCount(i);u++)
		{
			real err=0.125;
			real vmin=value[i][u]*(1.0-err);
			real vmax=value[i][u]*(1.0+err);
			EXPECT_EQ(vmin<=Q[i][u],true);
			EXPECT_EQ(Q[i][u]<=vmax,true);
		}
	}
	//各価値関数の値を表示する場合はif文をtrueにする
	if(false)
	{
		//状態価値関数の取得
		vector<real> J;
		smdp.getStateValueFunction(J);

		for(idx i=0;i<sp.getStateCount();i++)
		{
			cout<<i<<": ";
			for(idx u=0;u<sp.getControlCount(i);u++)
			{
				cout<<value[i][u]<<", ";
			}
			cout<<"\t";
			for(idx u=0;u<sp.getControlCount(i);u++)
			{
				cout<<Q[i][u]<<", ";
			}
			cout<<"\t";
			cout<<J[i];
			cout<<endl;
		}
	}

}

TEST_P(EpsilonSoftOnPolicyMonteCarloTest,PolicyImprovement)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	EpsilonSoftOnPolicyMonteCarlo<SimpleMDP> mc(smdp);

	//optimal policyから変化させたpolicyの
	//control-value functionのImprovement policyが改善されている（u=0）ことを確認する
	for(idx state=1;state<smdp.getStateCount()-1;state++)
	{
		//試験対象のpolicy
		//RegularPolicyから変位させる
		Policy p;
		smdp.getRegularPolicy(p);
		idx control=1;
		p.setPolicy(state,control);

		//value function
		vector<vector<real> > Q;
		//MDPにpを設定する
		smdp.setCurrentPolicy(p);
		smdp.getControlValueFunction(Q);
		//改善されたpolicyを格納する変数
		Policy p1;
		mc.policyImprovement(Q,p1);
		//変位したことを確認する
		EXPECT_EQ(p1[state],0);
	}
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// RL::EV3LineTracer
/////////////////////////////////////////////////////////////////////
TEST(EV3LineTracerTest,Constructor)
{
	EXPECT_NO_THROW(EV3LineTracer ev3);
	EXPECT_NO_THROW(EV3LineTracer ev3("/notexist.ini"));

}
TEST(EV3LineTracerTest,init)
{
	//デフォルトコンストラクタの場合→例外発生
	EV3LineTracer ev3_0;
	EXPECT_THROW(ev3_0.init(),std::ios_base::failure);
	//存在しないファイル名を指定したコンストラクタの場合→例外発生
	EV3LineTracer ev3_1("/notexist.ini");
	EXPECT_THROW(ev3_1.init(),std::ios_base::failure);


	//ファイルが存在する場合は例外発生なし
	EV3LineTracer ev3_2("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	//EXPECT_NO_THROW(ev3_2.Init());
	ev3_2.init();
	EXPECT_EQ(ev3_2.getInterval(),11);
	EXPECT_EQ(ev3_2.getStateCount(),10);
	EXPECT_EQ(ev3_2.getControlCount(0),1);
	EXPECT_EQ(ev3_2.getControlCount(1),2);

}

TEST(EV3LineTracerTest,Init_Exception)
{
	//デフォルトコンストラクタの場合→例外発生
	const idx testcount=23;
	vector<string> configfilepath(testcount);

	configfilepath[ 0]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR000_FormatIdentifier.ini";
	configfilepath[ 1]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR100_StateCount.ini";
	configfilepath[ 2]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR101_StateCount_UnnecessaryData.ini";
	configfilepath[ 3]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR200_State_StateIndex.ini";
	configfilepath[ 4]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR201_State_RefMax.ini";
	configfilepath[ 5]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR202_State_ControlCount.ini";
	configfilepath[ 6]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR203_State_UnnecessaryData.ini";
	configfilepath[ 7]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR204_State_InvalidData1.ini";
	configfilepath[ 8]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR205_State_InvalidData2.ini";
	configfilepath[ 9]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR300_Control_ControlIndex.ini";
	configfilepath[10]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR301_Control_StateIndex.ini";
	configfilepath[11]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR302_Control_LMotorSpeed.ini";
	configfilepath[12]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR303_Control_RMotorSpeed.ini";
	configfilepath[13]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR304_Control_UnnecessaryData.ini";
	configfilepath[14]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR305_Control_InvalidData1.ini";
	configfilepath[15]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR306_Control_InvalidData2.ini";
	configfilepath[16]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR400_RegularPolicy_StateIndex.ini";
	configfilepath[17]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR401_RegularPolicy_Control.ini";
	configfilepath[18]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR402_RegularPolicy_UnnecessaryData.ini";
	configfilepath[19]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR403_RegularPolicy_InvalidData1.ini";
	configfilepath[20]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR404_RegularPolicy_InvalidData2.ini";
	configfilepath[21]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR500_RegularPolicy_InvalidData.ini";
	configfilepath[22]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR501_RegularPolicy_UnnecessaryData.ini";


	for(idx i=0;i<testcount;i++)
	{
		EV3LineTracer ev3(configfilepath[i]);
		EXPECT_ANY_THROW(ev3.init());
	}

}

TEST(EV3LineTracerTest,getRegularPolicy)
{
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	Policy p;
	ev3.getRegularPolicy(p);
	idx statecount=p.getStateCount();
	vector<idx> c(statecount);
	c[0]=0;
	c[1]=1;
	c[2]=1;
	c[3]=0;
	c[4]=0;
	c[5]=1;
	c[6]=1;
	c[7]=0;
	c[8]=1;
	c[9]=1;

	for(idx i=0;i<statecount;i++)
	{
		EXPECT_EQ(p[i],c[i]);
	}
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// RL::TSVOutputContext
/////////////////////////////////////////////////////////////////////
TEST(TSVOutputContextTest,Constructor)
{
	EXPECT_NO_THROW(RL::TSVOutputContext toc(std::ostringstream()));

}

TEST(TSVOutputContextTest,writeToken)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	toc.writeToken("AAA");
	os.flush();
	EXPECT_TRUE(0==os.str().compare("AAA"));
}
TEST(TSVOutputContextTest,newLine)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	toc.writeToken("AAA");
	toc.newLine();
	toc.writeToken("BBB");
	os.flush();
	EXPECT_TRUE(0==os.str().compare("AAA\nBBB"));
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// RL::TSVInputContext
/////////////////////////////////////////////////////////////////////
TEST(TSVInputContextTest,Constructor)
{
	EXPECT_NO_THROW(RL::TSVInputContext tic(std::istringstream()));

}

TEST(TSVInputContextTest,nextToken1)
{
	string aaa="AAA";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	string s=tic.nextToken();
	EXPECT_TRUE(0==s.compare(aaa));
}
TEST(TSVInputContextTest,nextToken2)
{
	string teststring="AAA\tBBB\tCCC";
	std::istringstream is(teststring);
	RL::TSVInputContext tic(is);
	string s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("AAA"));
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("BBB"));
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("CCC"));
}
TEST(TSVInputContextTest,nextToken3)
{
	string teststring="AAA\tB B\tC  ";
	std::istringstream is(teststring);
	RL::TSVInputContext tic(is);
	string s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("AAA"));
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("B B"));
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("C  "));
}
TEST(TSVInputContextTest,skipToken)
{
	string teststring="AAA\tBBB\tCCC\tDDD";
	std::istringstream is(teststring);
	RL::TSVInputContext tic(is);
	EXPECT_NO_THROW(tic.skipToken("AAA"));
	EXPECT_NO_THROW(tic.skipToken("BBB"));
	EXPECT_NO_THROW(tic.skipToken("CCC"));
	EXPECT_THROW(tic.skipToken("ZZZ"), std::exception);
}
TEST(TSVInputContextTest,skipToken_Exception)
{
	string teststring="AAA\tBBB\tCCC";
	std::istringstream is(teststring);
	RL::TSVInputContext tic(is);
	EXPECT_NO_THROW(tic.skipToken("AAA"));
	EXPECT_THROW(tic.skipToken("DDD"), std::exception);
}
TEST(TSVInputContextTest,skipReturn)
{
	string aaa="AAA\nBBB";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	string s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("AAA"));
	tic.skipReturn();
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("BBB"));
}
TEST(TSVInputContextTest,hasNextToken)
{
	string aaa="AAA\tBBB\nCCC";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	EXPECT_TRUE(tic.hasNextToken());
	string s=tic.nextToken();//"AAA"を取得
	EXPECT_TRUE(tic.hasNextToken());
	s=tic.nextToken();//"BBB"を取得
	EXPECT_FALSE(tic.hasNextToken());
	tic.skipReturn();//2行目に移動
	EXPECT_TRUE(tic.hasNextToken());
	s=tic.nextToken();//"CCC"を取得
	EXPECT_FALSE(tic.hasNextToken());
}

TEST(TSVInputContextTest,deleteComment)
{
	string aaa="AAA\tBBB\tCCC\t\t;Comment\tComment";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	EXPECT_TRUE(tic.hasNextToken());
	string s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("AAA"));
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("BBB"));
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("CCC"));
	EXPECT_TRUE(false==tic.hasNextToken());
}
TEST(TSVInputContextTest,deleteComment2)
{
	string aaa="AAA\tBBB\tCCC\t\t   ;Comment\tComment";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	EXPECT_TRUE(tic.hasNextToken());
	string s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("AAA"));
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("BBB"));
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("CCC"));
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare(""));
	s=tic.nextToken();
	EXPECT_TRUE(0==s.compare("   "));
	EXPECT_TRUE(false==tic.hasNextToken());
}

TEST(ReadIntervalTest,Constractor)
{
	string aaa="10\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadInterval ri(ev3);
}
TEST(ReadIntervalTest,Process)
{
	string aaa="10\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadInterval ri(ev3);
	ri.process(tic);
	EXPECT_EQ(ev3.getInterval(),10);
}
TEST(ReadIntervalTest,Process_Exception)
{
	string aaa="aa\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadInterval ri(ev3);
	EXPECT_THROW(ri.process(tic),std::ios_base::failure);
}

TEST(ReadCostMaxTest,Process1)
{
	string aaa="10.125\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	RL::ReadCostMax rcm(ev3);
	rcm.process(tic);
	EXPECT_EQ(ev3.getCostMax(),10.125);
}
TEST(ReadCostMaxTest,Process2)
{
	string aaa="600\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	RL::ReadCostMax rcm(ev3);
	rcm.process(tic);
	EXPECT_EQ(ev3.getCostMax(),600);
}
TEST(ReadCostMaxTest,Process_Exception)
{
	string aaa="aa\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	RL::ReadCostMax rcm(ev3);
	EXPECT_THROW(rcm.process(tic),std::ios_base::failure);
}


TEST(ReadStateCountTest,Process)
{
	string aaa="11\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadStateCount rsc(ev3);
	rsc.process(tic);
	EXPECT_EQ(ev3.getStateCount(),11);
}
TEST(ReadStateCountTest,Process_Exception)
{
	string aaa="aa\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadStateCount rsc(ev3);
	EXPECT_THROW(rsc.process(tic),std::ios_base::failure);
}
TEST(ReadSingleStateTest,Process)
{

	string aaa="11\n10\n4	0.5	2\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadInterval(ev3).process(tic);
	ReadStateCount(ev3).process(tic);
	ReadSingleState rs(ev3,4);
	rs.process(tic);
	EXPECT_NEAR(ev3.getState(4).refMax,0.5,0.0625);
	EXPECT_EQ(ev3.getControlCount(4),2);
}
TEST(ReadSingleStateTest,Process_Exception)
{
	string aaa="aa\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadSingleState rs(ev3);
	EXPECT_THROW(rs.process(tic),std::ios_base::failure);
}
TEST(ReadStateTest,Process)
{

	string aaa="11\n2\n0	0.5	2\n1	0.5	2\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadInterval(ev3).process(tic);
	ReadStateCount(ev3).process(tic);
	ReadState rs(ev3);
	rs.process(tic);
	EXPECT_NEAR(ev3.getState(0).refMax,0.5,0.0625);
	EXPECT_NEAR(ev3.getState(1).refMax,0.5,0.0625);
	EXPECT_EQ(ev3.getControlCount(0),2);
	EXPECT_EQ(ev3.getControlCount(1),2);
}
TEST(ReadStateTest,Process_Exception)
{
	string aaa="10\naa\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	RL::ReadStateCount(ev3).process(tic);
	ReadState rs(ev3);
	EXPECT_THROW(rs.process(tic),std::ios_base::failure);
}

TEST(ReadControlTest,Process)
{

	string aaa="11\n10\n4	0.5	2\n";
	aaa+="4	0	10	 5\n";
	aaa+="4	1	 5	10\n";

	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadInterval(ev3).process(tic);
	ReadStateCount(ev3).process(tic);
	ReadSingleState(ev3,4).process(tic);
	ReadSingleControl rsc(ev3);
	rsc.setIndex(4,0);
	rsc.process(tic);
	rsc.setIndex(4,1);
	rsc.process(tic);
	EV3LineTracerControl ev3control[2];
	ev3control[0] = ev3.getControl(4,0);
	ev3control[1] = ev3.getControl(4,1);
	EXPECT_EQ(ev3control[0].lMotorSpeed,10);
	EXPECT_EQ(ev3control[0].rMotorSpeed, 5);
	EXPECT_EQ(ev3control[1].lMotorSpeed, 5);
	EXPECT_EQ(ev3control[1].rMotorSpeed,10);
}
TEST(ReadControlTest,Process_Exception)
{
	string aaa="aa\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadSingleControl rc(ev3);
	EXPECT_THROW(rc.process(tic),std::ios_base::failure);
}

TEST(InputConfigFileTest,Constractor)
{
	string aaa="EV3LineTracer_1.0\n10\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	RL::InputConfigFile icf(ev3);
}
TEST(InputConfigFileTest,process)
{
	string aaa="EV3LineTracer_1.0\n";
	aaa += "10\n";
	aaa += "600.0\n";
	aaa += "2\n";
	aaa += "0	0.1	1\n";
	aaa += "1	0.2	2\n";
	aaa += "0	0	10	100\n";
	aaa += "1	0	20	200\n";
	aaa += "1	1	21	201\n";
	aaa += "0	0\n";
	aaa += "1	1\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	RL::InputConfigFile icf(ev3);
	icf.process(tic);
	EXPECT_EQ(ev3.getInterval(),10);
	EXPECT_EQ(ev3.getCostMax(),600.0);
	EXPECT_EQ(ev3.getStateCount(),2);
	EXPECT_EQ(ev3.getControlCount(0),1);
	EXPECT_EQ(ev3.getControlCount(1),2);
	EXPECT_EQ(ev3.getControl(0,0).lMotorSpeed, 10);
	EXPECT_EQ(ev3.getControl(0,0).rMotorSpeed,100);
	EXPECT_EQ(ev3.getControl(1,0).lMotorSpeed, 20);
	EXPECT_EQ(ev3.getControl(1,0).rMotorSpeed,200);
	EXPECT_EQ(ev3.getControl(1,1).lMotorSpeed, 21);
	Policy p(ev3.getStateCount());
	ev3.getRegularPolicy(p);
	EXPECT_EQ(p.getStateCount(),2);
	EXPECT_EQ(p[0],0);
	EXPECT_EQ(p[1],1);

}
TEST(InputConfigFileTest,process_INIFile)
{
	ifstream ifs("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	RL::TSVInputContext tic(ifs);
	RL::EV3LineTracer ev3;
	RL::InputConfigFile icf(ev3);
	icf.process(tic);
	EXPECT_EQ(ev3.getInterval()  ,11);
	EXPECT_EQ(ev3.getCostMax()  ,600.0);
	EXPECT_EQ(ev3.getStateCount(),10);
	EXPECT_EQ(ev3.getState(4).refMax,0.5);
	EXPECT_EQ(ev3.getControl(9,1).lMotorSpeed, 5);
	EXPECT_EQ(ev3.getControl(9,1).rMotorSpeed,10);
	Policy p(ev3.getStateCount());
	EXPECT_EQ(ev3.getRegularPolicy(p)[9],1);

}


//値をパラメータ化したテストのためのクラスの定義
class InputConfigFileErrorTest : public ::testing::Test , public ::testing::WithParamInterface<int>
{
protected:
	vector<string> configfilepath;
	void SetUp(){
		configfilepath.resize(24);
		configfilepath[ 0]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR000_FormatIdentifier.ini";
		configfilepath[ 1]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR100_StateCount.ini";
		configfilepath[ 2]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR101_StateCount_UnnecessaryData.ini";
		configfilepath[ 3]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR200_State_StateIndex.ini";
		configfilepath[ 4]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR201_State_RefMax.ini";
		configfilepath[ 5]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR202_State_ControlCount.ini";
		configfilepath[ 6]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR203_State_UnnecessaryData.ini";
		configfilepath[ 7]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR204_State_InvalidData1.ini";
		configfilepath[ 8]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR205_State_InvalidData2.ini";
		configfilepath[ 9]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR300_Control_ControlIndex.ini";
		configfilepath[10]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR301_Control_StateIndex.ini";
		configfilepath[11]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR302_Control_LMotorSpeed.ini";
		configfilepath[12]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR303_Control_RMotorSpeed.ini";
		configfilepath[13]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR304_Control_UnnecessaryData.ini";
		configfilepath[14]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR305_Control_InvalidData1.ini";
		configfilepath[15]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR306_Control_InvalidData2.ini";
		configfilepath[16]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR400_RegularPolicy_StateIndex.ini";
		configfilepath[17]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR401_RegularPolicy_Control.ini";
		configfilepath[18]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR402_RegularPolicy_UnnecessaryData.ini";
		configfilepath[19]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR403_RegularPolicy_InvalidData1.ini";
		configfilepath[20]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR404_RegularPolicy_InvalidData2.ini";
		configfilepath[21]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR500_RegularPolicy_InvalidData.ini";
		configfilepath[22]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR501_RegularPolicy_UnnecessaryData.ini";
		configfilepath[23]="/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_ERR501_RegularPolicy_UnnecessaryData.ini";
	}
	void TearDown(){}
public:
	InputConfigFileErrorTest():
		configfilepath()
	{
	}
};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(
		InstantiateInputConfigFileErrorTest,
		InputConfigFileErrorTest,
		::testing::Range(0, 24,1)
);


TEST_P(InputConfigFileErrorTest,process_INIFile_error)
{
	ifstream ifs(configfilepath[GetParam()].c_str());
	RL::TSVInputContext tic(ifs);
	RL::EV3LineTracer ev3;
	RL::InputConfigFile icf(ev3);
	EXPECT_ANY_THROW(icf.process(tic));
}

TEST(ExecNullCommandTest,process)
{
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	ev3.execNullCommand();
}

TEST(WriteIntervalTest,Constractor)
{
	RL::EV3LineTracer ev3;
	RL::WriteInterval wi(ev3);
}

TEST(WriteIntervalTest,Process)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::WriteInterval wi(ev3);
	wi.process(toc);
	EXPECT_EQ(os.str(),std::to_string(ev3.getInterval())+"\n");
}
TEST(WriteCostMaxTest,Constractor)
{
	RL::EV3LineTracer ev3;
	RL::WriteCostMax wcm(ev3);
}

TEST(WriteCostMaxTest,Process)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::WriteCostMax wcm(ev3);
	wcm.process(toc);
	EXPECT_EQ(os.str(),std::to_string(ev3.getCostMax())+"\n");
}

TEST(WriteStateCountTest,Constractor)
{
	RL::EV3LineTracer ev3;
	RL::WriteStateCount wsc(ev3);
}

TEST(WriteStateCountTest,Process)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	RL::WriteStateCount wsc(ev3);
	ev3.init();
	wsc.process(toc);
	EXPECT_EQ(os.str(),std::to_string(ev3.getStateCount())+"\n");
}
TEST(WriteSingleStateTest,Constractor)
{
	RL::EV3LineTracer ev3;
	RL::WriteSingleState wss(ev3,0);
}

TEST(WriteSingleStateTest,Process)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	idx i=ev3.getStateCount()/2;
	RL::WriteSingleState wss(ev3,i);
	wss.process(toc);
	EXPECT_EQ(
			os.str(),
			std::to_string(i)
				+'\t'
				+std::to_string(ev3.getState(i).refMax)
				+'\t'
				+std::to_string(ev3.getControlCount(i))
				+"\n"
			);
}
TEST(WriteStateTest,Constractor)
{
	RL::EV3LineTracer ev3;
	RL::WriteState wss(ev3);
}

TEST(WriteStateTest,Process)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	//テスト対象：WriteState
	RL::WriteState wss(ev3);
	//書き込み処理の実行
	wss.process(toc);
	//出力される想定の文字列
	std::ostringstream state_string;
	//state数
	int state_count = ev3.getStateCount();
	//出力される想定の文字列の作成
	for(int i=0; i<state_count; i++)
	{
		state_string<<std::to_string(i);
		state_string<<'\t';
		state_string<<std::to_string(ev3.getState(i).refMax);
		state_string<<'\t';
		state_string<<ev3.getControlCount(i);
		state_string<<endl;
	}
	//想定通りの文字列が出力されているか確認する
	EXPECT_EQ(os.str(),state_string.str());
}


TEST(WriteSingleControlTest,Constractor)
{
	RL::EV3LineTracer ev3;
	RL::WriteSingleControl wsc(ev3,0,0);
}

TEST(WriteSingleControlTest,Process)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	idx i=ev3.getStateCount()/2;
	idx u=ev3.getControlCount(i)/2;
	RL::WriteSingleControl wsc(ev3,i,u);
	wsc.process(toc);
	EV3LineTracerControl control = ev3.getControl(i,u);
	EXPECT_EQ(
			os.str(),
			std::to_string(i)
				+'\t'
				+std::to_string(u)
				+'\t'
				+std::to_string(control.lMotorSpeed)
				+'\t'
				+std::to_string(control.rMotorSpeed)
				+"\n"
			);
}


TEST(WriteControlTest,Constractor)
{
	RL::EV3LineTracer ev3;
	RL::WriteControl wc(ev3);
}

TEST(WriteControlTest,Process)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	//テスト対象：WriteState
	RL::WriteControl wc(ev3);
	//書き込み処理の実行
	wc.process(toc);
	//出力される想定の文字列
	std::ostringstream state_string;
	//state数
	idx state_count = ev3.getStateCount();
	//出力される想定の文字列の作成
	for(idx i=0; i<state_count; i++)
	{
		idx control_count = ev3.getControlCount(i);
		for(idx u = 0; u<control_count; u++)
		{
			EV3LineTracerControl control = ev3.getControl(i,u);

			state_string<<std::to_string(i);
			state_string<<'\t';
			state_string<<std::to_string(u);
			state_string<<'\t';
			state_string<<std::to_string(control.lMotorSpeed);
			state_string<<'\t';
			state_string<<std::to_string(control.rMotorSpeed);
			state_string<<endl;
		}
	}
	//想定通りの文字列が出力されているか確認する
	EXPECT_EQ(os.str(),state_string.str());
}

TEST(WriteSinglePolicyTest,Constractor)
{
	RL::Policy policy;
	RL::WriteSinglePolicy wsp(policy,0);
}

TEST(WriteSinglePolicyTest,Process)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	idx i=ev3.getStateCount()/2;
	Policy policy;
	ev3.getRegularPolicy(policy);
	RL::WriteSinglePolicy wsp(policy,i);
	wsp.process(toc);
	EXPECT_EQ(
			os.str(),
			std::to_string(i)
				+'\t'
				+std::to_string(policy[i])
				+"\n"
			);
}
TEST(WriteRegularPolicyTest,Constractor)
{
	RL::EV3LineTracer ev3;
	RL::WriteRegularPolicy<EV3LineTracer> wrp(ev3);
}

TEST(WritePolicyTest,Process)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	//テスト対象：RegularPolicy
	RL::WriteRegularPolicy<EV3LineTracer> wrp(ev3);
	//書き込み処理の実行
	wrp.process(toc);
	//出力される想定の文字列
	std::ostringstream state_string;
	//state数
	int state_count = ev3.getStateCount();
	Policy regular_policy;
	ev3.getRegularPolicy(regular_policy);
	//出力される想定の文字列の作成
	for(int i=0; i<state_count; i++)
	{
		state_string<<std::to_string(i);
		state_string<<'\t';
		state_string<<std::to_string(regular_policy[i]);
		state_string<<endl;
	}
	//想定通りの文字列が出力されているか確認する
	EXPECT_EQ(os.str(),state_string.str());
}
TEST(OutputCommandNullCommandTest,Constractor)
{
	//OutputContextの初期化
	RL::OutputCommandNullCommand onc();
}

TEST(OutputCommandNullCommandTest,Process)
{
	//OutputContextの初期化
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::OutputCommandNullCommand onc;
	//処理の実行
	onc.process(toc);

	EXPECT_EQ(os.str(),"NullCommand\n");
}
TEST(OutputEV3LineTracer_1_0Test,Constractor)
{
	//OutputContextの初期化
	RL::OutputCommandNullCommand onc;
	RL::OutputEV3LineTracer_1_0 oelt10(onc);
}

TEST(OutputEV3LineTracer_1_0Test,Process)
{
	//OutputContextの初期化
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::OutputCommandNullCommand onc;
	RL::OutputEV3LineTracer_1_0 oelt10(onc);

	//処理の実行
	oelt10.process(toc);

	//出力される想定の文字列
	ostringstream output_string;
	output_string<<"EV3LineTracer_1.0";
	output_string<<endl;
	output_string<<"NullCommand";
	output_string<<endl;
	//想定通りの文字列が出力されているか確認する
	EXPECT_EQ(os.str(),output_string.str());
}
TEST(OutputMessageTest,Constractor)
{
	//OutputContextの初期化
	RL::OutputCommandNullCommand onc;
	RL::OutputEV3LineTracer_1_0 oelt10(onc);
	RL::OutputMessage_1_0 om(oelt10);
}

TEST(OutputMessageTest,Process)
{
	//OutputContextの初期化
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::OutputCommandNullCommand onc;
	RL::OutputEV3LineTracer_1_0 oelt10(onc);
	RL::OutputMessage_1_0 om(oelt10);

	//処理の実行
	om.process(toc);

	//出力される想定の文字列
	ostringstream output_string;
	output_string<<"MESSAGE_1.0";
	output_string<<endl;
	output_string<<"EV3LineTracer_1.0";
	output_string<<endl;
	output_string<<"NullCommand";
	output_string<<endl;
	output_string<<endl;

	//想定通りの文字列が出力されているか確認する
	EXPECT_EQ(os.str(),output_string.str());
}

TEST(InputCommandNullCommandTest,Constractor)
{
	//InputCommandNullCommandの初期化
	RL::InputCommandNullCommand inc;
}

TEST(InputCommandNullCommandTest,Process)
{
	//InputCommandNullCommandの初期化
	std::istringstream is("NullCommand\nOK\n");
	RL::TSVInputContext tic(is);
	RL::InputCommandNullCommand inc;
	//処理の実行
	inc.process(tic);

}
TEST(InputEV3LineTracer_1_0Test,Constractor)
{
	//InputEV3LineTracer_1_0の初期化
	RL::InputCommandNullCommand inc;
	RL::InputEV3LineTracer_1_0 iev3(inc);

}

TEST(InputEV3LineTracer_1_0Test,Process)
{
	//InputContextの初期化
	std::istringstream is("EV3LineTracer_1.0\nNullCommand\nOK\n");
	RL::TSVInputContext tic(is);
	RL::InputCommandNullCommand inc;
	RL::InputEV3LineTracer_1_0 iev3(inc);
	//処理の実行
	iev3.process(tic);

}

TEST(InputMessage_1_0Test,Constractor)
{
	//InputMessage_1_0の初期化
	RL::InputCommandNullCommand inc;
	RL::InputEV3LineTracer_1_0 iev3(inc);
	RL::InputMessage_1_0 im(iev3);
}

TEST(InputMessage_1_0Test,Process)
{
	//InputContextの初期化
	std::istringstream is("MESSAGE_1.0\nEV3LineTracer_1.0\nNullCommand\nOK\n\n");
	RL::TSVInputContext tic(is);
	RL::InputCommandNullCommand inc;
	RL::InputEV3LineTracer_1_0 iev3(inc);
	RL::InputMessage_1_0 im(iev3);

	//処理の実行
	im.process(tic);

}
//OutputCommandSetMDP
TEST(OutputCommandSetMDPTest,Constractor)
{
	//OutputContextの初期化
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::OutputCommandSetMDP o_setmdp(ev3);
}

TEST(OutputCommandSetMDPTest,Process)
{

	string expect_string = "SetMDP\n\
11\n\
600.000000\n\
10\n\
0	0.100000	1\n\
1	0.200000	2\n\
2	0.300000	2\n\
3	0.400000	2\n\
4	0.500000	2\n\
5	0.600000	2\n\
6	0.700000	2\n\
7	0.800000	2\n\
8	0.900000	2\n\
9	1.000000	2\n\
0	0	10	10\n\
1	0	10	5\n\
1	1	5	10\n\
2	0	10	5\n\
2	1	5	10\n\
3	0	10	5\n\
3	1	5	10\n\
4	0	10	5\n\
4	1	5	10\n\
5	0	10	5\n\
5	1	5	10\n\
6	0	10	5\n\
6	1	5	10\n\
7	0	10	5\n\
7	1	5	10\n\
8	0	10	5\n\
8	1	5	10\n\
9	0	10	5\n\
9	1	5	10\n\
0	0\n\
1	1\n\
2	1\n\
3	0\n\
4	0\n\
5	1\n\
6	1\n\
7	0\n\
8	1\n\
9	1\n\
";


	//OutputContextの初期化
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::OutputCommandSetMDP o_setmdp(ev3);


	//処理の実行
	o_setmdp.process(toc);

	EXPECT_EQ(os.str(),expect_string);
}
TEST(InputCommandSetMDPTest,Constractor)
{
	///OutputContextの初期化
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::InputCommandSetMDP i_setmdp(ev3);
}

TEST(InputCommandSetMDPTest,Process_OK)
{
	//InputContextの初期化
	std::string input_string="SetMDP\nOK\n\
11\n\
600.000000\n\
10\n\
0	0.100000	1\n\
1	0.200000	2\n\
2	0.300000	2\n\
3	0.400000	2\n\
4	0.500000	2\n\
5	0.600000	2\n\
6	0.700000	2\n\
7	0.800000	2\n\
8	0.900000	2\n\
9	1.000000	2\n\
0	0	10	10\n\
1	0	10	5\n\
1	1	5	10\n\
2	0	10	5\n\
2	1	5	10\n\
3	0	10	5\n\
3	1	5	10\n\
4	0	10	5\n\
4	1	5	10\n\
5	0	10	5\n\
5	1	5	10\n\
6	0	10	5\n\
6	1	5	10\n\
7	0	10	5\n\
7	1	5	10\n\
8	0	10	5\n\
8	1	5	10\n\
9	0	10	5\n\
9	1	5	10\n\
0	0\n\
1	1\n\
2	1\n\
3	0\n\
4	0\n\
5	1\n\
6	1\n\
7	0\n\
8	1\n\
9	1\n\
";
	std::istringstream is(input_string);
	RL::TSVInputContext tic(is);
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::InputCommandSetMDP i_setmdp(ev3);

	//処理の実行
	i_setmdp.process(tic);

}
TEST(InputCommandSetMDPTest,Process_NG)
{
	//InputContextの初期化
	std::istringstream is("SetMDP\nNG\n");
	RL::TSVInputContext tic(is);
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::InputCommandSetMDP i_setmdp(ev3);

	//処理の実行
	EXPECT_THROW(i_setmdp.process(tic),std::ios_base::failure);

}
TEST(InputCommandSetMDPTest,Process_error)
{
	//InputContextの初期化
	std::istringstream is("SetMDP\nXXX\n");
	RL::TSVInputContext tic(is);
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::InputCommandSetMDP i_setmdp(ev3);

	//処理の実行
	EXPECT_THROW(i_setmdp.process(tic),std::ios_base::failure);
}

TEST(ExecSetMDPTest,process)
{
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	ev3.execSetMDP();
}

TEST(ReadStepTest,Process)
{
	string i_string="5	3	4	0.5\n";
	std::istringstream is(i_string);
	RL::TSVInputContext tic(is);
	RL::EpisodeData episode_data(10);
	RL::Episode episode(episode_data);
	RL::ReadStep(episode,5).process(tic);
	EXPECT_EQ(episode[5].state,3);
	EXPECT_EQ(episode[5].control,4);
	EXPECT_EQ(episode[5].cost,0.5);
}

//値をパラメータ化したテストのためのクラスの定義
class ReadStepErrorTest : public ::testing::Test , public ::testing::WithParamInterface<int>
{
protected:
	vector<string> i_string;
	void SetUp(){
		i_string.resize(5);
		i_string[ 0]="aa	3	4	0.5\n";
		i_string[ 1]="5	aa	4	0.5\n";
		i_string[ 2]="5	3	aa	0.5\n";
		i_string[ 3]="5	3	4	aa\n";
		i_string[ 4]="5	3	4	0.5";

	}
	void TearDown(){}
public:
	ReadStepErrorTest():
		i_string()
	{
	}
};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(
		InstantiateReadStepErrorTest,
		ReadStepErrorTest,
		::testing::Range(0, 5,1)
);


TEST_P(ReadStepErrorTest,process_error)
{
	std::istringstream is(i_string[GetParam()]);
	RL::TSVInputContext tic(is);
	RL::EpisodeData episode_data(10);
	RL::Episode episode;
	EXPECT_THROW(RL::ReadStep(episode,5).process(tic),std::ios_base::failure);
}


TEST(ReadEpisodeTest,Process)
{
	std::stringstream string("");
	string << "7" << endl;
	string << "0	10	20	0.30" << endl;
	string << "1	11	21	0.31" << endl;
	string << "2	12	22	0.32" << endl;
	string << "3	13	23	0.33" << endl;
	string << "4	14	24	0.34" << endl;
	string << "5	15	25	0.35" << endl;
	string << "6	16	26	0.36" << endl;
	RL::TSVInputContext tic(string);
	RL::Episode episode;
	RL::ReadEpisode(episode).process(tic);
	EXPECT_EQ(episode[0].state,  10);
	EXPECT_EQ(episode[0].control,20);
	EXPECT_EQ(episode[0].cost,    0.30);
	EXPECT_EQ(episode[3].state,  13);
	EXPECT_EQ(episode[3].control,23);
	EXPECT_EQ(episode[3].cost,    0.33);
	EXPECT_EQ(episode[6].state,  16);
	EXPECT_EQ(episode[6].control,26);
	EXPECT_EQ(episode[6].cost,    0.36);
}
TEST(ReadEpisodeTest,Process_Error)
{
	std::stringstream string("");
	string << "7" << endl;
	string << "0	10	20	0.30" << endl;
	string << "1	11	21	0.31" << endl;
	string << "2	12	22	0.32" << endl;
	string << "5	15	25	0.35" << endl;
	string << "4	14	24	0.34" << endl;
	string << "3	13	23	0.33" << endl;
	string << "6	16	26	0.36" << endl;
	RL::TSVInputContext tic(string);
	RL::Episode episode;
	EXPECT_THROW(RL::ReadEpisode(episode).process(tic),std::ios_base::failure);
}

TEST(InputCommandExecEpisodeTest,Constractor)
{
	//InputCommandNullCommandの初期化
	RL::InputCommandExecEpisode icee(Episode());
}

TEST(InputCommandExecEpisodeTest,Process)
{
	//InputCommandNullCommandの初期化
	std::stringstream ss("");
	ss << "ExecEpisode" << endl;
	ss << "OK" << endl;
	ss << "7" << endl;
	ss << "0	10	20	0.30" << endl;
	ss << "1	11	21	0.31" << endl;
	ss << "2	12	22	0.32" << endl;
	ss << "3	13	23	0.33" << endl;
	ss << "4	14	24	0.34" << endl;
	ss << "5	15	25	0.35" << endl;
	ss << "6	16	26	0.36" << endl;
	RL::TSVInputContext tic(ss);
	RL::Episode episode;
	RL::InputCommandExecEpisode icee(episode);
	//処理の実行
	icee.process(tic);

	EXPECT_EQ(episode.getStepCount(),7);
	EXPECT_EQ(episode[0].state,  10);
	EXPECT_EQ(episode[0].control,20);
	EXPECT_EQ(episode[0].cost,    0.30);
	EXPECT_EQ(episode[3].state,  13);
	EXPECT_EQ(episode[3].control,23);
	EXPECT_EQ(episode[3].cost,    0.33);
	EXPECT_EQ(episode[6].state,  16);
	EXPECT_EQ(episode[6].control,26);
	EXPECT_EQ(episode[6].cost,    0.36);
}
TEST(InputCommandExecEpisodeTest,Process_NG)
{
	//InputCommandNullCommandの初期化
	std::stringstream ss("");
	ss << "ExecEpisode" << endl;
	ss << "NG" << endl;
	ss << "REASONREASONREASON" << endl;
	ss << "REASONREASONREASON" << endl;
	ss << "REASONREASONREASON" << endl;
	ss << "REASONREASONREASON" << endl;
	RL::TSVInputContext tic(ss);
	RL::Episode episode;
	RL::InputCommandExecEpisode icee(episode);
	//処理の実行
	EXPECT_THROW( icee.process(tic),std::ios_base::failure);
}

TEST(OutputCommandExecEpisodeTest,Constractor)
{
	//OutputContextの初期化
	RL::OutputCommandExecEpisode ocee;
}

TEST(OutputCommandExecEpisodeTest,Process)
{
	//OutputContextの初期化
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::OutputCommandExecEpisode ocee;
	//処理の実行
	ocee.process(toc);

	EXPECT_EQ(os.str(),"ExecEpisode\n");
}
TEST(GetEpisodeTest,Process)
{
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_execEpisode.ini");
	ev3.init();
	RL::Episode episode;
	ev3.getEpisode(episode);
	EXPECT_TRUE(episode.getStepCount()>0);
}

TEST(ReadStochasticPolicyTest,constructor)
{

	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	StochasticPolicy sp(ev3);
	RL::ReadStochasticPolicy rsp(sp);
}
TEST(ReadStochasticPolicyTest,process)
{
	std::stringstream string("");
	string << "0	1.0" << endl;
	string << "1	0.5	0.5" << endl;
	string << "2	0.5	0.5" << endl;
	string << "3	0.5	0.5" << endl;
	string << "4	0.5	0.5" << endl;
	string << "5	0.5	0.5" << endl;
	string << "6	0.5	0.5" << endl;
	string << "7	0.5	0.5" << endl;
	string << "8	0.5	0.5" << endl;
	string << "9	0.5	0.5" << endl;
	RL::TSVInputContext tic(string);

	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	StochasticPolicy sp(ev3);
	RL::ReadStochasticPolicy rsp(sp);
	rsp.process(tic);
}
TEST(ReadStochasticPolicyTest,process_Error)
{
	std::stringstream string("");
	string << "0	1.0" << endl;
	string << "1	0.5	0.5" << endl;
	string << "2	0.5	0.5" << endl;
	string << "3	0.5	0.5" << endl;
	string << "4	0.5	0.5" << endl;
	string << "5	0.0	0.0" << endl;//確率の合計が0.0になる
	string << "6	0.5	0.5" << endl;
	string << "7	0.5	0.5" << endl;
	string << "8	0.5	0.5" << endl;
	string << "9	0.5	0.5" << endl;
	RL::TSVInputContext tic(string);

	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	StochasticPolicy sp(ev3);
	RL::ReadStochasticPolicy rsp(sp);

	EXPECT_ANY_THROW(rsp.process(tic));

}

TEST(WriteStochasticPolicyTest,Process)
{
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	StochasticPolicy sp;
	ev3.getStochasticPolicy(ev3.getRegularPolicy(),sp);
	//テスト対象：WriteStochasticPolicy
	RL::WriteStochasticPolicy wsp(sp);
	//書き込み処理の実行
	wsp.process(toc);
	//出力される想定の文字列
	std::ostringstream state_string;
	//state数
	idx state_count = ev3.getStateCount();

	//出力される想定の文字列の作成
	for(idx i=0; i<state_count; i++)
	{
		state_string<<std::to_string(i);
		idx control_count = sp[i].getValueMax();
		for(idx u = 0; u < control_count; u++)
		{
			state_string<<'\t';
			state_string<<std::to_string(sp[i].getProbability(u));
		}
		state_string<<endl;
	}
	//想定通りの文字列が出力されているか確認する
	EXPECT_EQ(os.str(),state_string.str());
}


TEST(ReadCurrentPolicyTest,Process_SimpleMDP)
{
	//テスト用のMDP
	SimpleMDP mdp(5);
	mdp.init();
	//読み込む文字列
	stringstream string("");
	string << "0	1.00" << endl;
	string << "1	0.25	0.75" << endl;
	string << "2	0.25	0.75" << endl;
	string << "3	0.25	0.75" << endl;
	string << "4	1.00" << endl;
	//InputContext
	TSVInputContext tic(string);
	//テスト対象
	RL::ReadCurrentPolicy<RL::SimpleMDP> rcp(mdp);

	//読み込み実行
	rcp.process(tic);

	//CurrentPolicyを取得
	StochasticPolicy sp(mdp.getCurrentPolicy());
	//許容誤差
	real e = 0.0625*0.0625*0.0625*0.0625;
	//読み込みに成功しているかを確認する
	EXPECT_NEAR(sp[0].getProbability(0), 1.0 , e);
	EXPECT_NEAR(sp[1].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[1].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[2].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[2].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[3].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[3].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[4].getProbability(0), 1.00, e);
}
TEST(ReadCurrentPolicyTest,Process_EV3LineTracer)
{
	//テスト用のMDP
	EV3LineTracer mdp("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	mdp.init();
	//読み込む文字列
	stringstream string("");
	string << "0	1.00" << endl;
	string << "1	0.25	0.75" << endl;
	string << "2	0.25	0.75" << endl;
	string << "3	0.25	0.75" << endl;
	string << "4	0.25	0.75" << endl;
	string << "5	0.25	0.75" << endl;
	string << "6	0.25	0.75" << endl;
	string << "7	0.25	0.75" << endl;
	string << "8	0.25	0.75" << endl;
	string << "9	0.25	0.75" << endl;
	//InputContext
	TSVInputContext tic(string);
	//テスト対象
	RL::ReadCurrentPolicy<RL::EV3LineTracer> rcp(mdp);

	//読み込み実行
	rcp.process(tic);

	//CurrentPolicyを取得
	StochasticPolicy sp(mdp.getCurrentPolicy());
	//許容誤差
	real e = 0.0625*0.0625*0.0625*0.0625;
	//読み込みに成功しているかを確認する
	EXPECT_NEAR(sp[0].getProbability(0), 1.0 , e);
	EXPECT_NEAR(sp[1].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[1].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[2].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[2].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[3].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[3].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[4].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[4].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[5].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[5].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[6].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[6].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[7].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[7].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[8].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[8].getProbability(1), 0.75, e);
	EXPECT_NEAR(sp[9].getProbability(0), 0.25, e);
	EXPECT_NEAR(sp[9].getProbability(1), 0.75, e);
}

TEST(WriteCurrentPolicyTest,Construstor_SimpleMDP)
{
	RL::SimpleMDP mdp(5);
	mdp.init();
	RL::WriteCurrentPolicy<RL::SimpleMDP> wcp(mdp);
}
TEST(WriteCurrentPolicyTest,Process_SimpleMDP)
{
	RL::SimpleMDP mdp(5);
	mdp.init();
	RL::WriteCurrentPolicy<RL::SimpleMDP> wcp(mdp);

	ostringstream oss;
	RL::TSVOutputContext toc(oss);

	wcp.process(toc);

	stringstream expect_string("");
	expect_string << "0	1.000000" << endl;
	expect_string << "1	1.000000	0.000000" << endl;
	expect_string << "2	1.000000	0.000000" << endl;
	expect_string << "3	1.000000	0.000000" << endl;
	expect_string << "4	1.000000" << endl;


	EXPECT_EQ(oss.str(),expect_string.str());
}
TEST(WriteCurrentPolicyTest,Construstor_EV3LineTracer)
{
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::WriteCurrentPolicy<RL::EV3LineTracer> wcp(ev3);
}
TEST(WriteCurrentPolicyTest,Process_EV3LineTracer)
{
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::WriteCurrentPolicy<RL::EV3LineTracer> wcp(ev3);

	ostringstream oss;
	RL::TSVOutputContext toc(oss);

	wcp.process(toc);

	stringstream expect_string("");
	expect_string << "0	1.000000" << endl;
	expect_string << "1	0.000000	1.000000" << endl;
	expect_string << "2	0.000000	1.000000" << endl;
	expect_string << "3	1.000000	0.000000" << endl;
	expect_string << "4	1.000000	0.000000" << endl;
	expect_string << "5	0.000000	1.000000" << endl;
	expect_string << "6	0.000000	1.000000" << endl;
	expect_string << "7	1.000000	0.000000" << endl;
	expect_string << "8	0.000000	1.000000" << endl;
	expect_string << "9	0.000000	1.000000" << endl;

	EXPECT_EQ(oss.str(),expect_string.str());
}

//OutputCommandCurrentPolicy
TEST(OutputCommandCurrentPolicyTest,Constractor)
{
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::OutputCommandSetCurrentPolicy ocp(ev3);

}


TEST(OutputCommandCurrentPolicyTest,Process_EV3LineTracer)
{

	ostringstream expect_string("");
	expect_string << "SetCurrentPolicy" << endl;
	expect_string << "0	1.000000" << endl;
	expect_string << "1	0.000000	1.000000" << endl;
	expect_string << "2	0.000000	1.000000" << endl;
	expect_string << "3	1.000000	0.000000" << endl;
	expect_string << "4	1.000000	0.000000" << endl;
	expect_string << "5	0.000000	1.000000" << endl;
	expect_string << "6	0.000000	1.000000" << endl;
	expect_string << "7	1.000000	0.000000" << endl;
	expect_string << "8	0.000000	1.000000" << endl;
	expect_string << "9	0.000000	1.000000" << endl;

	//OutputContextの初期化
	std::ostringstream os;
	RL::TSVOutputContext toc(os);
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::OutputCommandSetCurrentPolicy ocp(ev3);


	//処理の実行
	ocp.process(toc);

	EXPECT_EQ(os.str(),expect_string.str());
}
TEST(InputCommandSetCurrentPolicyTest,Constractor)
{
	///OutputContextの初期化
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::InputCommandSetCurrentPolicy iscp(ev3);
}

TEST(InputCommandSetCurrentPolicyTest,Process_OK)
{
	//InputContextの初期化
	std::stringstream input_string("");
	input_string << "SetCurrentPolicy" << endl;
	input_string << "OK" << endl;
	input_string << "0	1.000000" << endl;
	input_string << "1	0.000000	1.000000" << endl;
	input_string << "2	0.000000	1.000000" << endl;
	input_string << "3	1.000000	0.000000" << endl;
	input_string << "4	1.000000	0.000000" << endl;
	input_string << "5	0.000000	1.000000" << endl;
	input_string << "6	0.000000	1.000000" << endl;
	input_string << "7	1.000000	0.000000" << endl;
	input_string << "8	0.000000	1.000000" << endl;
	input_string << "9	0.000000	1.000000" << endl;

	std::istringstream is(input_string.str());
	RL::TSVInputContext tic(is);
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::InputCommandSetCurrentPolicy iscp(ev3);

	//処理の実行
	iscp.process(tic);

}
TEST(InputCommandSetCurrentPolicyTest,Process_NG)
{
	//InputContextの初期化
	std::istringstream is("SetCurrentPolicy\nNG\n");
	RL::TSVInputContext tic(is);
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	RL::InputCommandSetCurrentPolicy iscp(ev3);

	//処理の実行
	EXPECT_THROW(iscp.process(tic),std::ios_base::failure);

}
//StochasticPolicyに丸め誤差が生じる場合
TEST(InputCommandSetCurrentPolicyTest,Process_OK2)
{

	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();

	//ev3にCurrentPolicyとして設定する
	//StochasticPlicyの設定
	vector<vector<real> > prob(10);
	prob[0].resize(1);
	for(idx i = 1; i < 10; i++)
	{
		prob[i].resize(2);
	}
	prob[0][0]=1.0;
	prob[1][0]=0.1;prob[1][1]=0.9;
	prob[2][0]=0.2;prob[2][1]=0.8;
	prob[3][0]=0.3;prob[3][1]=0.7;
	prob[4][0]=0.4;prob[4][1]=0.6;
	prob[5][0]=0.5;prob[5][1]=0.5;
	prob[6][0]=0.6;prob[6][1]=0.4;
	prob[7][0]=0.7;prob[7][1]=0.3;
	prob[8][0]=0.8;prob[8][1]=0.2;
	prob[9][0]=0.9;prob[9][1]=0.1;
	RL::StochasticPolicy cp(prob);

	//ev3へのCurrentPolicyの設定
	ev3.setCurrentPolicy(cp);

	//InputContextの初期化
	std::stringstream input_string("");
	input_string << "SetCurrentPolicy" << endl;
	input_string << "OK" << endl;
	input_string << "0	1.000000" << endl;
	input_string << "1	0.100000	0.900000" << endl;
	input_string << "2	0.200000	0.800000" << endl;
	input_string << "3	0.300000	0.700000" << endl;
	input_string << "4	0.400000	0.600000" << endl;
	input_string << "5	0.500000	0.500000" << endl;
	input_string << "6	0.600000	0.400000" << endl;
	input_string << "7	0.700000	0.300000" << endl;
	input_string << "8	0.800000	0.200000" << endl;
	input_string << "9	0.900000	0.100000" << endl;


	std::istringstream is(input_string.str());
	RL::TSVInputContext tic(is);
	RL::InputCommandSetCurrentPolicy iscp(ev3);

	//処理の実行
	iscp.process(tic);

}

TEST(ExecSetCurrentPolicyTest,process)
{
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.init();
	ev3.setCurrentPolicy(ev3.getRegularPolicy());
}


TEST(ActualMachineTest,EpsilonSoftOnPolicyMonteCarlo)
{
	RL::EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer_execEpisode.ini");

	EpsilonSoftOnPolicyMonteCarlo<RL::EV3LineTracer> esopmc(ev3);

	Policy p;
	esopmc.policyIteration(p,10,5);
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////


//単体テストの実行
int main(int argc, char** argv)
{

	//::testing::GTEST_FLAG(filter)="*RandomIdxTest*";
	//::testing::GTEST_FLAG(filter)="*EV3LineTracerTest*";
	//::testing::GTEST_FLAG(filter)="*TSVOutputContext*";
	//::testing::GTEST_FLAG(filter)="*Input*";
	//::testing::GTEST_FLAG(filter)="*Read*";
	//::testing::GTEST_FLAG(filter)="*INI*";
	//::testing::GTEST_FLAG(filter)="*Null*:*TSVInputContextTest*:*EV3LineTracerTest*";
	//::testing::GTEST_FLAG(filter)="*Write*";
	//::testing::GTEST_FLAG(filter)="*Input*:*Output*";
	//::testing::GTEST_FLAG(filter)="*EpsilonSoftOnPolicyMonteCarlo*";
	::testing::GTEST_FLAG(filter)="-*ActualMachineTest*";



	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}


