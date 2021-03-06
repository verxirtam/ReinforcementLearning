/*
 * SimpleMDP.cpp
 *
 *  Created on: 2014/01/19
 *      Author: daisuke
 */

#include "SimpleMDP.h"

using namespace std;

namespace RL
{


SimpleMDP::SimpleMDP(idx length):
	stateCount(length),//状態数の初期化
	transitionProb(0.75),//RegularPolicyがterminate stateに向かう推移確率
	prob(),//(state,control)に対する推移確率を格納するvector
	currentPolicy()//現在設定されているPolicy
{
	//(state,control)に対する推移確率を格納するvector
	std::vector<std::vector<std::vector<real> > > prob0;


	//各Controlの推移確率を格納する
	prob0.resize(stateCount);
	//Prob0[0](terminate state)の設定(control数:1)
	//自分自身に推移するcontrolを1つのみ持つ
	prob0[0].resize(1);
	prob0[0][0].resize(stateCount);
	prob0[0][0][0]=1.0;
	for(idx j=1;j<prob0[0][0].size();j++)
	{
		prob0[0][0][j]=0.0;
	}
	//Prob[i][u]の設定(control数:2)
	//各controlの推移確率を以下になるように設定
	//control0  i-1:0.75 i+1:0.25 その他:0.0
	//control1  i-1:0.25 i+1:0.75 その他:0.0
	for(idx i=1;i<prob0.size()-1;i++)
	{
		prob0[i].resize(2);
		for(idx u=0;u<prob0[i].size();u++)
		{
			prob0[i][u].resize(stateCount);
			for(idx j=0;j<prob0[i][u].size();j++)
			{
				prob0[i][u][j]=0.0;
			}
		}
		prob0[i][0][i-1]=transitionProb;
		prob0[i][0][i+1]=1.0-transitionProb;
		prob0[i][1][i-1]=1.0-transitionProb;
		prob0[i][1][i+1]=transitionProb;
	}
	//Prob[StateCount-1]の設定(control数:1)
	//control0の推移確率を以下になるように設定
	//control0  i-1:1.0 その他:0.0
	prob0[stateCount-1].resize(1);
	prob0[stateCount-1][0].resize(stateCount);
	for(idx k=0;k<prob0[stateCount-1][0].size();k++)
	{
		prob0[stateCount-1][0][k]=0.0;
	}
	prob0[stateCount-1][0][stateCount-2]=1.0;

	//Probの初期化
	const idx part_count=16;
	prob.resize(stateCount);
	for(idx i=0;i<stateCount;i++)
	{
		prob[i].resize(prob0[i].size());
		for(idx u=0;u<prob0[i].size();u++)
		{
			prob[i][u]=RandomIdx(prob0[i][u],part_count);
		}
	}

	//CurrentPolicyの設定
	Policy p;
	this->setCurrentPolicy(this->getRegularPolicy(p));
}

//valuefunctionを3重対角行列の解法を用いて求める
vector<real>& SimpleMDP::getStateValueFunction(vector<real>& J)const
{
	//ベルマン方程式J=TJ ⇔ J=g+PJ ⇔ (1-P)J=g
	//を3重対角行列の解法を用いて解く

	J=vector<real>(stateCount);

	//対角成分の下側
	vector<real> a(stateCount);
	//対角成分(一定値(=1)のためreal変数)
	real bi=1.0;
	//対角成分の上側
	vector<real> c(stateCount);
	//方程式の右辺=g
	vector<real> d(stateCount);


	//変数の初期化
	J[0]=0.0;
	a[0]=0.0;
	c[0]=0.0;
	d[0]=0.0;
	for(idx i=1;i<stateCount-1;i++)
	{
		real pi0=currentPolicy[i].getProbability(0);
		real pi1=currentPolicy[i].getProbability(1);
		a[i]=-pi0*transitionProb       - pi1*(1.0-transitionProb);
		c[i]=-pi0*(1.0-transitionProb) - pi1*transitionProb;
		d[i]=1.0;
	}
	a[stateCount-1]=-1.0;
	c[stateCount-1]= 0.0;
	d[stateCount-1]= 1.0;

	//a[]=0となるように式変形
	//c[1]=c[1]/bi;
	//d[1]=d[1]/bi;
	for(idx i=1;i<stateCount;i++)
	{
		real m=1.0/(bi-c[i-1]*a[i]);
		c[i]=c[i]*m;
		d[i]=(d[i]-d[i-1]*a[i])*m;
	}

	//解の導出
	J[stateCount-1]=d[stateCount-1];
	for(idx i=stateCount-2;i>0;i--)
	{
		J[i]=d[i]-c[i]*J[i+1];
	}

	return J;
}
//control-value function を反復法で求める
//引数：Q 結果を格納するvector
//戻り値：引数Qの参照
vector<vector<real> >& SimpleMDP::getControlValueFunction(vector<vector<real> >& Q)const
{
	//解を格納するvector Q[state][control]
	Q=vector<vector<real> >(stateCount);

	//Qの初期化 Q[i][u]=iで初期化する
	for(idx i=0;i<stateCount;i++)
	{
		Q[i]=vector<real>(this->getControlCount(i),i);
	}

	//MDPの推移確率とpolicyの選択確率を事前計算する
	vector<vector<real> > pi(stateCount);//選択確率
	vector<vector<vector<real> > > prob_(stateCount);//推移確率
	for(idx i=0;i<stateCount;i++)
	{
		idx umax=this->getControlCount(i);
		pi[i].resize(umax);
		prob_[i].resize(umax);
		for(idx u=0;u<umax;u++)
		{
			pi[i][u]=currentPolicy[i].getProbability(u);
			if(i==0)
			{
				prob_[i][u]=vector<real>(1,0.0);
				prob_[i][u][0]=prob[i][u].getProbability(i);
			}
			else if(i==stateCount-1)
			{
				prob_[i][u]=vector<real>(1,0.0);
				prob_[i][u][0]=prob[i][u].getProbability(i-1);
			}
			else
			{
				prob_[i][u]=vector<real>(2,0.0);
				prob_[i][u][0]=prob[i][u].getProbability(i-1);
				prob_[i][u][1]=prob[i][u].getProbability(i+1);
			}
		}
	}


	//i=0のときはQ[i][u]=0.0
	Q[0][0]=0.0;

	//control-value Bellman eq.の反復適用を行う
	//反復回数の最大値
	const idx kmax=100;
	for(idx k=0;k<kmax;k++)
	{
		for(idx i=1;i<stateCount;i++)
		{
			idx umax=this->getControlCount(i);
			for(idx u=0;u<umax;u++)
			{
				//i-1に対するコスト(=m0)の計算
				real m0=0.0;
				if(i>1)
				{
					//i>1の場合
					// (i=1の場合はm0=0)
					idx vmax=this->getControlCount(i-1);
					for(idx v=0;v<vmax;v++)
					{
						m0+=pi[i-1][v]*Q[i-1][v];
					}
					m0*=prob_[i][u][0];
				}
				//i+1に対するコスト(=m1)の計算
				real m1=0.0;
				if(i<stateCount-1)
				{
					//i<StateCount-1の場合
					// (i=StateCount-1の場合はm1=0)
					idx vmax=this->getControlCount(i+1);
					for(idx v=0;v<vmax;v++)
					{
						m1+=pi[i+1][v]*Q[i+1][v];
					}
					m1*=prob_[i][u][1];
				}
				//g(i,u)=1と、i-1とi+1に対するコストの和(=Q[i][u])を求める
				Q[i][u]=1.0+m0+m1;
			}//u
		}//i
	}//k

	return Q;
}


}
