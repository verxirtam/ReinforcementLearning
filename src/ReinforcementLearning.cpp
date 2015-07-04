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
#include "EV3LineTracer/EV3LineTracer.h"
#include "Communication/TSVOutputContext.h"
#include "Communication/TSVInputContext.h"
#include "EV3LineTracer/InputEV3Linetracer_1_0.h"
#include "EV3LineTracer/InputConfigFile.h"
#include "EV3LineTracer/Communication/Read/ReadSingleControl.h"
#include "EV3LineTracer/Communication/Read/ReadSingleState.h"
#include "EV3LineTracer/Communication/Read/ReadStateCount.h"

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
	EXPECT_EQ(0,p0.GetStateCount());
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
	p0.SetPolicy(0,1);
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
	EXPECT_EQ(p0.GetStateCount(),p1.GetStateCount());
	EXPECT_EQ(p0[0],p1[0]);
	//コピー元の値を変更する
	p0.SetPolicy(0,1);
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
	p1.SetPolicy(size/2,2);
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
		p.SetPolicy(i,i);
	}
	//サイズのチェック
	EXPECT_EQ(size,p.GetStateCount());
	//setterで設定した値が反映されているかのチェック
	for(i=0;i<size;i++)
	{
		EXPECT_EQ(i,p[i]);
	}
	//添字演算子は例外を返さない
	EXPECT_NO_THROW(p[size]);
	//setterは例外を返す
	EXPECT_THROW(p.SetPolicy(size,0),std::out_of_range);
	//setterは例外を返す
	EXPECT_THROW(p.SetPolicy(-1,0),std::out_of_range);
	//サイズのチェック
	EXPECT_EQ(size,p.GetStateCount());
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
		p.SetPolicy(i,i);
	}
	for(i=0;i<size;i++)
	{
		EXPECT_EQ(i,p[i]);
	}
	for(i=0;i<size;i++)
	{
		p.SetPolicy(i,2*i);
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
	EXPECT_EQ(statecount,sp.GetStateCount());
	for(i=0;i<statecount;i++)
	{
		EXPECT_EQ(controlcount[i],sp.GetControlCount(i));
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
	EXPECT_EQ(0,sp.GetStateCount());
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
	vector<real> prob(p0[m].GetValueMax(),0.0);
	prob[0]=0.5;
	prob[1]=0.5;
	p0.SetProbability(m,prob);
	EXPECT_NO_THROW(p0.Correct());
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
	vector<real> prob(p0[m].GetValueMax(),0.0);
	prob[0]=0.5;
	prob[1]=0.5;
	p0.SetProbability(m,prob);
	EXPECT_NO_THROW(p0.Correct());
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
	vector<real> prob(p0[m].GetValueMax(),0.0);
	prob[0]=0.5;
	prob[1]=0.5;
	p1.SetProbability(m,prob);
	p1.Correct();
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
	vector<real> prob(p0[m].GetValueMax(),0.0);
	prob[0]=0.5;
	prob[1]=0.5;
	p0.SetProbability(m,prob);
	EXPECT_EQ(p0[m].GetProbability(0),0.5);
	EXPECT_EQ(p0[m].GetProbability(1),0.5);

	prob=vector<real>(p0[m].GetValueMax(),0.0);
	prob[0]= 1.0;
	prob[1]=-1.0;
	EXPECT_THROW(p0.SetProbability(m,prob),std::invalid_argument);

	prob=vector<real>(p0[m].GetValueMax(),0.0);
	prob[0]=1.0;
	prob[1]=1.0;
	prob[2]=1.0;
	prob[3]=1.0;
	p0.SetProbability(m,prob);
	p0.Correct(m);
	EXPECT_EQ(p0[m].GetProbability(0),0.25);
	EXPECT_EQ(p0[m].GetProbability(1),0.25);
	EXPECT_EQ(p0[m].GetProbability(2),0.25);
	EXPECT_EQ(p0[m].GetProbability(3),0.25);
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
	EXPECT_NO_THROW(p0.SetProbability(i,prob));
	EXPECT_EQ(p0[i].GetProbability(0),0.5);
	EXPECT_EQ(p0[i].GetProbability(1),0.5);


	StochasticPolicy p1(controlcount);

	prob=vector<real>(35,0);
	prob[0]=1.0;
	prob[1]=1.0;
	i=statecount/2;
	EXPECT_THROW(p1.SetProbability(i,prob),std::out_of_range);
	EXPECT_EQ(p1[i].GetProbability(0),1.0);
	EXPECT_EQ(p1[i].GetProbability(1),0.0);
}

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//RL::Stepのテスト
/////////////////////////////////////////////////////////////////////


TEST(StepTest,EqualityOperator)
{
	Step step0;
	step0.State=0;
	step0.Control=0;
	step0.Cost=0.0;

	Step step1=step0;

	EXPECT_EQ(step0==step1,true);
	EXPECT_EQ(step0!=step1,false);


	step1.State=1;

	EXPECT_EQ(step0==step1,false);
	EXPECT_EQ(step0!=step1,true);

	step1=step0;
	step1.Control=1;

	EXPECT_EQ(step0==step1,false);
	EXPECT_EQ(step0!=step1,true);

	step1=step0;
	step1.Cost=1.0;

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
			s.State  =i%13;
			s.Control=i%5;
			s.Cost   =i%103;
			Steps.push_back(s);
		}
	}
	void TearDown(){}


};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(InstantiateEpisodeTest,EpisodeTest,::testing::Values(1,10,50,100));


//コンストラクタのテスト
TEST_P(EpisodeTest,Constructor0)
{
	idx imax=GetParam();
	Episode episode0(Steps);
	EXPECT_EQ(episode0.GetStepCount(),imax);
	for(idx i=0;i<episode0.GetStepCount();i++)
	{
		EXPECT_EQ(episode0[i].State  ,i% 13);
		EXPECT_EQ(episode0[i].Control,i%  5);
		EXPECT_EQ(episode0[i].Cost   ,i%103);
	}
}

TEST(EpisodeTest,init_exception)
{
}

//デフォルトコンストラクタのテスト
TEST(EpisodeTest,DefaultConstructor)
{
	Episode episode0;
	EXPECT_EQ(episode0.GetStepCount(),0);
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
	episode0.SetStep(imax/2,14,0,0);
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
	episode0.SetStep(imax/2,14,0,0);
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
		Steps[i].State  =Steps[i].State+1;
		Steps[i].Control=Steps[i].Control+1;
		Steps[i].Cost   =Steps[i].Cost+1;
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
	episode1.SetStep(0,0,0,-1.0);
	//等号演算子の実行(falseとなる)
	EXPECT_EQ(episode0==episode1,false);
	EXPECT_EQ(episode0!=episode1,true);
}

//SetStep()のテスト
TEST_P(EpisodeTest,SetStep)
{
	//episode[i_mod]の内容をSetStep()で変更して、正しく変更されたかを確認する

	Episode episode0(Steps);
	Step step;
	idx i_mod=imax/2;
	step.State  =Steps[i_mod].State+1;
	step.Control=Steps[i_mod].Control+1;
	step.Cost   =Steps[i_mod].Cost+1.0;

	episode0.SetStep(i_mod,step);

	EXPECT_EQ(episode0[i_mod],step);

	step.State  =Steps[i_mod].State+1;
	step.Control=Steps[i_mod].Control+1;
	step.Cost   =Steps[i_mod].Cost+1.0;

	episode0.SetStep(i_mod,step);

	EXPECT_EQ(episode0[i_mod],step);
}

//AddStep()のテスト
TEST_P(EpisodeTest,AddStep)
{
	Episode episode0(Steps);
	Step step;

	//引数の設定
	step.State  =500;
	step.Control=500;
	step.Cost   =500.0;

	//AddStep()の実行
	episode0.AddStep(step.State,step.Control,step.Cost);

	//StepCountが1増加していることを確認
	EXPECT_EQ(episode0.GetStepCount(),imax+1);
	//末尾のStepがstepと一致していることを確認
	EXPECT_EQ(episode0[imax],step);

	//Stepの値を変更
	step.State  =1000;
	step.Control=1000;
	step.Cost   =1000.0;

	//AddStep()の実行
	episode0.AddStep(step);

	//StepCountが1増加していることを確認
	EXPECT_EQ(episode0.GetStepCount(),imax+2);
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
	EXPECT_EQ(smdp.GetStateCount(),2);
	EXPECT_EQ(smdp.GetControlCount(0),1);
	EXPECT_EQ(smdp.GetControlCount(1),1);
}

TEST_P(SimpleMDPTest,Constructor1)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	EXPECT_EQ(smdp.GetStateCount(),statecount);


	EXPECT_EQ(smdp.GetControlCount(0),1);
	for(idx i=1;i<statecount-1;i++)
	{
		EXPECT_EQ(smdp.GetControlCount(i),2);
	}
	EXPECT_EQ(smdp.GetControlCount(statecount-1),1);

}

TEST_P(SimpleMDPTest,GetPolicy)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	Policy p;
	smdp.GetPolicy(p);
	EXPECT_EQ(p.GetStateCount(),statecount);
}
TEST_P(SimpleMDPTest,GetRegularPolicy)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	Policy p;
	smdp.GetRegularPolicy(p);
	EXPECT_EQ(p.GetStateCount(),statecount);
	for(idx i=0;i<p.GetStateCount();i++)
	{
		EXPECT_EQ(p[i],0);
	}
}

TEST_P(SimpleMDPTest,GetDiscountRate)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	EXPECT_EQ(smdp.GetDiscountRate(),1.0);
}

TEST_P(SimpleMDPTest,GetNextStep)
{
	const idx statecount=GetParam();
	SimpleMDP smdp(statecount);
	//EXPECT_NE(smdp.GetEpisode().GetStepCount(),0);
	Policy p;
	smdp.GetRegularPolicy(p);
	Step s;
	const idx trycount=50000;
	//state毎に推移確率を推定して実際の確率を近似していることを確認する
	for(idx i=0;i<statecount;i++)
	{
		//推移した回数をカウントするvector
		vector<idx> stat(statecount,0);
		s.State=i;
		s.Control=p[i];
		s.Cost=1.0;
		//試行を行いstate毎に推移した回数をカウントする
		for(idx t=0;t<trycount;t++)
		{
			Step next=smdp.GetNextStep(s);
			stat[next.State]++;
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
		smdp.GetEpisode(e);
		idx stepcount=e.GetStepCount();
		//S[][], N[]に集計する
		for(idx n=0;n<stepcount-1;n++)
		{
			S[e[n].State][e[n+1].State]++;
			N[e[n].State]++;
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
	smdp.GetStochasticPolicy(smdp.GetRegularPolicy(p),sp);
	for(idx i=0;i<sp.GetStateCount();i++)
	{
		vector<real> prob(sp.GetControlCount(i));
		for(idx u=0;u<sp.GetControlCount(i);u++)
		{
			prob[u]=1.0+Random::UniformReal();
		}
		sp.SetProbability(i,prob);
	}
	smdp.SetCurrentPolicy(sp);
	//S[i][j]:state i の後state jに遷移した回数
	vector<vector<idx> > S(statecount,vector<idx>(statecount,0));
	//N[i]stateiを訪れた回数
	vector<idx> N(statecount,0);
	//エピソードの取得を繰り返す
	for(idx i=0;i<10000;i++)
	{
		Episode e;
		smdp.GetEpisode(e);
		idx stepcount=e.GetStepCount();
		//S[][], N[]に集計する
		for(idx n=0;n<stepcount-1;n++)
		{
			S[e[n].State][e[n+1].State]++;
			N[e[n].State]++;
		}
	}

	//集計した結果を表示する
	for(idx i=0;i<statecount;i++)
	{
		real val0=sp[i].GetProbability(0)*0.75
				+sp[i].GetProbability(1)*0.25;
		real val1=sp[i].GetProbability(0)*0.25
				+sp[i].GetProbability(1)*0.75;
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

TEST_P(RandomTest,UniformIdx)
{
	idx max=GetParam();
	vector<idx> stat(max);
	const idx testcount=10000;

	Random::Init();

	for(idx n=0;n<testcount;n++)
	{
		idx i=Random::UniformIdx(max);
		stat[i]++;
	}
	for(idx i=0;i<max;i++)
	{
		EXPECT_NEAR((real)stat[i]/(real)testcount,1.0/(double)max,0.01);
	}
}

TEST_P(RandomTest,UniformReal)
{
	idx max=GetParam();
	vector<idx> stat(max);
	const idx testcount=100000;

	Random::Init();

	for(idx n=0;n<testcount;n++)
	{
		real r=max*Random::UniformReal();
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
		real d=Random::UniformReal();
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
		idx i=ri.Get();
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

	EXPECT_EQ(ri.GetValueMax(),max);
	for(idx i=0;i<prob.size();i++)
	{
		EXPECT_NEAR(ri.GetProbability(i),prob[i],0.125*0.125*0.125*0.125);
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
	mc.PolicyIteration(p,10,100);
	//value function
	vector<vector<real> > Q;
	smdp.SetCurrentPolicy(p);
	smdp.GetControlValueFunction(Q);

	//optimal policy と異なるポリシーの価値関数が最適とならないことを確認する。
	Policy p1=p;
	vector<vector<real> > Q1;
	Random::Init();
	idx state=Random::UniformIdx(p1.GetStateCount()-2)+1;
	idx control=1;//Random::UniformIdx(2);
	p1.SetPolicy(state,control);
	smdp.SetCurrentPolicy(p1);
	smdp.GetControlValueFunction(Q1);

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
	smdp.GetRegularPolicy(rp);
	real e=0.5;
	for(idx k=0;k<10;k++)
	{
		StochasticPolicy sp;
		mc.EpsilonGreedy(rp,e,sp);
		for(idx i=0;i<sp.GetStateCount();i++)
		{
			idx controlcount=sp.GetControlCount(i);
			for(idx u;u<controlcount;u++)
			{
				if(u==rp[i])
				{
					real prob=1.0 - e + e/((real)controlcount);
					EXPECT_NEAR(sp[i].GetProbability(u),prob,0.01);
				}
				else
				{
					real prob=e/((real)controlcount);
					EXPECT_NEAR(sp[i].GetProbability(u),prob,0.01);
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
	mc.EpsilonGreedy(smdp.GetRegularPolicy(p),e,sp);
	mc.SetCurrentPolicy(sp);
	vector<vector<real> > value;

	mc.PolicyEvaluation(value,10000);
	vector<vector<real> > Q;
	smdp.GetControlValueFunction(Q);

	for(idx i=0;i<sp.GetStateCount();i++)
	{
		for(idx u=0;u<sp.GetControlCount(i);u++)
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
		smdp.GetStateValueFunction(J);

		for(idx i=0;i<sp.GetStateCount();i++)
		{
			cout<<i<<": ";
			for(idx u=0;u<sp.GetControlCount(i);u++)
			{
				cout<<value[i][u]<<", ";
			}
			cout<<"\t";
			for(idx u=0;u<sp.GetControlCount(i);u++)
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
	for(idx state=1;state<smdp.GetStateCount()-1;state++)
	{
		//試験対象のpolicy
		//RegularPolicyから変位させる
		Policy p;
		smdp.GetRegularPolicy(p);
		idx control=1;
		p.SetPolicy(state,control);

		//value function
		vector<vector<real> > Q;
		//MDPにpを設定する
		smdp.SetCurrentPolicy(p);
		smdp.GetControlValueFunction(Q);
		//改善されたpolicyを格納する変数
		Policy p1;
		mc.PolicyImprovement(Q,p1);
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
TEST(EV3LineTracerTest,Init)
{
	//デフォルトコンストラクタの場合→例外発生
	EV3LineTracer ev3_0;
	EXPECT_THROW(ev3_0.Init(),std::ios_base::failure);
	//存在しないファイル名を指定したコンストラクタの場合→例外発生
	EV3LineTracer ev3_1("/notexist.ini");
	EXPECT_THROW(ev3_1.Init(),std::ios_base::failure);


	//ファイルが存在する場合は例外発生なし
	EV3LineTracer ev3_2("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	//EXPECT_NO_THROW(ev3_2.Init());
	ev3_2.Init();
	EXPECT_EQ(ev3_2.GetInterval(),11);
	EXPECT_EQ(ev3_2.GetStateCount(),10);
	EXPECT_EQ(ev3_2.GetControlCount(0),1);
	EXPECT_EQ(ev3_2.GetControlCount(1),2);

}

TEST(EV3LineTracerTest,Init_Exception)
{
	//デフォルトコンストラクタの場合→例外発生
	const idx testcount=23;
	vector<EV3LineTracer> ev3(testcount);
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
		ev3[i]=EV3LineTracer(configfilepath[i]);
		EXPECT_THROW(ev3[i].Init(),std::ios_base::failure);
	}

}

TEST(EV3LineTracerTest,GetRegularPolicy)
{
	EV3LineTracer ev3("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	ev3.Init();
	Policy p;
	ev3.GetRegularPolicy(p);
	idx statecount=p.GetStateCount();
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
	EXPECT_EQ(ev3.GetInterval(),10);
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
TEST(ReadStateCountTest,Process)
{
	string aaa="11\n";
	std::istringstream is(aaa);
	RL::TSVInputContext tic(is);
	RL::EV3LineTracer ev3;
	ReadStateCount rsc(ev3);
	rsc.process(tic);
	EXPECT_EQ(ev3.GetStateCount(),11);
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
	EXPECT_NEAR(ev3.GetState(4).RefMax,0.5,0.0625);
	EXPECT_EQ(ev3.GetControlCount(4),2);
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
	EXPECT_NEAR(ev3.GetState(0).RefMax,0.5,0.0625);
	EXPECT_NEAR(ev3.GetState(1).RefMax,0.5,0.0625);
	EXPECT_EQ(ev3.GetControlCount(0),2);
	EXPECT_EQ(ev3.GetControlCount(1),2);
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
	EXPECT_EQ(ev3control[0].LMotorSpeed,10);
	EXPECT_EQ(ev3control[0].RMotorSpeed, 5);
	EXPECT_EQ(ev3control[1].LMotorSpeed, 5);
	EXPECT_EQ(ev3control[1].RMotorSpeed,10);
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
	RL::InputEV3Linetracer_1_0 iev3(ev3);
	RL::InputConfigFile icf(iev3);
}
TEST(InputConfigFileTest,process)
{
	string aaa="EV3LineTracer_1.0\n10\n2\n";
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
	RL::InputEV3Linetracer_1_0 iev3(ev3);
	RL::InputConfigFile icf(iev3);
	icf.process(tic);
	EXPECT_EQ(ev3.GetInterval(),10);
	EXPECT_EQ(ev3.GetStateCount(),2);
	EXPECT_EQ(ev3.GetControlCount(0),1);
	EXPECT_EQ(ev3.GetControlCount(1),2);
	EXPECT_EQ(ev3.getControl(0,0).LMotorSpeed, 10);
	EXPECT_EQ(ev3.getControl(0,0).RMotorSpeed,100);
	EXPECT_EQ(ev3.getControl(1,0).LMotorSpeed, 20);
	EXPECT_EQ(ev3.getControl(1,0).RMotorSpeed,200);
	EXPECT_EQ(ev3.getControl(1,1).LMotorSpeed, 21);
	Policy p(ev3.GetStateCount());
	ev3.GetRegularPolicy(p);
	EXPECT_EQ(p.GetStateCount(),2);
	EXPECT_EQ(p[0],0);
	EXPECT_EQ(p[1],1);

}
TEST(InputConfigFileTest,process_INIFile)
{
	ifstream ifs("/home/daisuke/git/ReinforcementLearning/res/EV3LineTracer.ini");
	RL::TSVInputContext tic(ifs);
	RL::EV3LineTracer ev3;
	RL::InputEV3Linetracer_1_0 iev3(ev3);
	RL::InputConfigFile icf(iev3);
	icf.process(tic);
	EXPECT_EQ(ev3.GetInterval()  ,11);
	EXPECT_EQ(ev3.GetStateCount(),10);
	EXPECT_EQ(ev3.GetState(4).RefMax,0.5);
	EXPECT_EQ(ev3.getControl(9,1).LMotorSpeed, 5);
	EXPECT_EQ(ev3.getControl(9,1).RMotorSpeed,10);
	Policy p(ev3.GetStateCount());
	EXPECT_EQ(ev3.GetRegularPolicy(p)[9],1);

}


//値をパラメータ化したテストのためのクラスの定義
class InputConfigFileErrorTest : public ::testing::Test , public ::testing::WithParamInterface<int>
{
protected:
	vector<string> configfilepath;
	void SetUp(){
		configfilepath.resize(23);
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
	}
	void TearDown(){}
};

//パラメータの定義
//正常系のテストを行う要素数の定義
INSTANTIATE_TEST_CASE_P(
		InstantiateInputConfigFileErrorTest,
		InputConfigFileErrorTest,
		::testing::Range(0, 23,1)
);


TEST_P(InputConfigFileErrorTest,process_INIFile_error)
{
	ifstream ifs(configfilepath[GetParam()].c_str());
	RL::TSVInputContext tic(ifs);
	RL::EV3LineTracer ev3;
	RL::InputEV3Linetracer_1_0 iev3(ev3);
	RL::InputConfigFile icf(iev3);
	EXPECT_THROW(icf.process(tic),std::ios_base::failure);
}

TEST(ExecNullCommandTest,process)
{
	RL::EV3LineTracer ev3;
	ev3.Init();
	ev3.execNullCommand();
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


	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
