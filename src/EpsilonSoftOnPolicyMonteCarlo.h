/*
 * EpsilonSoftOnPolicyMonteCarlo.h
 *
 *  Created on: 2014/01/15
 *      Author: daisuke
 */

#ifndef EPSILONSOFTONPOLICYMONTECARLO_H_
#define EPSILONSOFTONPOLICYMONTECARLO_H_

#include "RLUtility.h"
#include "Policy.h"


namespace RL
{

/*
	class EpsilonSoftOnPolicyMonteCarlo
	イプシロンソフト方策オン型モンテカルロ法

	typename MDPに必要なメンバ関数
	Mdp.init()
	Mdp.getStateCount()const
	Mdp.getControlCount(i)
	Mdp.getRegularPolicy(out)
	Mdp.getEpisode(e)
	Mdp.getDiscountRate()
	Mdp.getCostMax()
	Mdp.getStochasticPolicy(p,out)
	Mdp.setCurrentPolicy(p)
	Mdp.setCurrentPolicy(sp)
 */
template <typename MDP>
class EpsilonSoftOnPolicyMonteCarlo
{
private:
	MDP mdp;
public:
	EpsilonSoftOnPolicyMonteCarlo(const MDP& mdp_):mdp(mdp_)
	{
	}
	//CurrentPolicyを
	//control-value functionに基づいて改善したPolicyを求める
	Policy& policyImprovement(const std::vector<std::vector<real> >& Q,Policy& out)const
	{
		//state数の取得
		idx statecount=mdp.getStateCount();
		//出力用変数の初期化
		out=Policy(statecount);

		//stateごとにcostが最小のcontrolを求める
		for(idx i=0;i<statecount;i++)
		{
			//最小のconst
			real Qmin=Q[i][0];
			//最小のcostを持つcontrol
			idx umin=0;
			//control数
			idx controlcount=mdp.getControlCount(i);
			//costが最小のcontrolを求める
			for(idx u=0;u<controlcount;u++)
			{
				//より小さいcostの場合は最小値を更新する
				if(Qmin>=Q[i][u])
				{
					Qmin=Q[i][u];
					umin=u;
				}
			}
			//最小のcostのcontrolを出力用ポリシーにセットする
			out.setPolicy(i,umin);
		}

		return out;
	}

	Policy& policyIteration(Policy& out,idx pi_count=10, idx episode_count=10000)
	{

		//MDPの初期化
		mdp.init();

		//e-greedy policy
		StochasticPolicy mu_e;
		//epsilonの初期化
		real e=0.125;

		//初期ポリシーの設定(RegularPolicy)
		mdp.getRegularPolicy(out);

		//Policy Iterationの実行
		for(idx k=0;k<pi_count;k++)
		{
			setCurrentPolicy(epsilonGreedy(out,e,mu_e));
			std::vector<std::vector<real> > Q;
			policyEvaluation(Q,episode_count);
			Policy policy_new;
			this->policyImprovement(Q,policy_new);
			if(out==policy_new)
			{
				return out;
			}
			out=policy_new;
		}

		return out;
	}
	std::vector<std::vector<real> >& policyEvaluation(std::vector<std::vector<real> >& value, idx episodecount=10000)const
	{
		//集計用変数((i,u)のコストを集計する)
		std::vector<std::vector<real> > S(mdp.getStateCount());
		//集計用変数((i,u)に遭遇した回数を集計)
		std::vector<std::vector<idx> > N(mdp.getStateCount());
		//0で初期化する
		for(idx i=0;i<S.size();i++)
		{
			S[i].resize(mdp.getControlCount(i),0.0);
			N[i].resize(mdp.getControlCount(i),0);
		}

		//Episodeの実行（最大episodecount回）
		for(idx k=0;k<episodecount;k++)
		{
			Episode e;
			bool result;
			result=mdp.getEpisode(e);
			//エピソード取得不可の場合は終了
			if(result==false)
			{
				break;
			}
			//コストの集計
			real t=0.0;
			for(idx m=e.getStepCount()-1;m>=0;m--)
			{
				t=e[m].cost+mdp.getDiscountRate()*t;
				S[e[m].state][e[m].control]+=t;
				N[e[m].state][e[m].control]++;
				if(m==0)
				{
					break;
				}
			}

		}

		value.resize(mdp.getStateCount());
		//価値観数の近似値の算出
		for(idx i=0;i<mdp.getStateCount();i++)
		{
			value[i].resize(mdp.getControlCount(i));

			for(idx u=0;u<mdp.getControlCount(i);u++)
			{
				if(N[i][u]!=0)
				{
					value[i][u]=S[i][u]/((real)N[i][u]);
				}
				else
				{
					value[i][u]=mdp.getCostMax();
				}
			}
		}
		return value;
	}
	StochasticPolicy& epsilonGreedy(const Policy& p,real e,StochasticPolicy& out)const
	{
		//policy pと同値なStochastic Policyを作成
		mdp.getStochasticPolicy(p,out);

		//p[]に対するe-greedyポリシーを作成する
		for(idx i=0;i<out.getStateCount();i++)
		{
			const idx controlcount=out[i].getValueMax();
			std::vector<real> prob(controlcount, 0.0);
			for(idx u=0;u<controlcount;u++)
			{
				if(u==p[i])
				{
					prob[u]=1.0 - e + e/((real)controlcount);
				}
				else
				{
					prob[u]=e/((real)controlcount);
				}
			}
			out.setProbability(i,prob);
		}

		return out;

	}
	void setCurrentPolicy(const Policy& p)
	{
		mdp.setCurrentPolicy(p);
	}
	void setCurrentPolicy(const StochasticPolicy& sp)
	{
		mdp.setCurrentPolicy(sp);
	}
};




}



#endif /* EPSILONSOFTONPOLICYMONTECARLO_H_ */
