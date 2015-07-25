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
	Mdp.GetStateCount()const
	Mdp.GetControlCount(i)
	Mdp.GetRegularPolicy(out)
	Mdp.GetEpisode(e)
	Mdp.GetDiscountRate()
	Mdp.GetCostMax()
	Mdp.GetStochasticPolicy(p,out)
	Mdp.SetCurrentPolicy(p)
	Mdp.SetCurrentPolicy(sp)
 */
template <typename MDP>
class EpsilonSoftOnPolicyMonteCarlo
{
private:
	MDP Mdp;
public:
	EpsilonSoftOnPolicyMonteCarlo(const MDP& mdp):Mdp(mdp)
	{
	}
	//CurrentPolicyを
	//control-value functionに基づいて改善したPolicyを求める
	Policy& PolicyImprovement(const std::vector<std::vector<real> >& Q,Policy& out)const
	{
		//state数の取得
		idx statecount=Mdp.GetStateCount();
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
			idx controlcount=Mdp.GetControlCount(i);
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
			out.SetPolicy(i,umin);
		}

		return out;
	}

	Policy& PolicyIteration(Policy& out,idx pi_count=10, idx episode_count=10000)
	{

		//MDPの初期化
		Mdp.Init();

		//e-greedy policy
		StochasticPolicy mu_e;
		//epsilonの初期化
		real e=0.125;

		//初期ポリシーの設定(RegularPolicy)
		Mdp.GetRegularPolicy(out);

		//Policy Iterationの実行
		for(idx k=0;k<pi_count;k++)
		{
			SetCurrentPolicy(EpsilonGreedy(out,e,mu_e));
			std::vector<std::vector<real> > Q;
			PolicyEvaluation(Q,episode_count);
			Policy policy_new;
			this->PolicyImprovement(Q,policy_new);
			if(out==policy_new)
			{
				return out;
			}
			out=policy_new;
		}

		return out;
	}
	std::vector<std::vector<real> >& PolicyEvaluation(std::vector<std::vector<real> >& value, idx episodecount=10000)const
	{
		//集計用変数((i,u)のコストを集計する)
		std::vector<std::vector<real> > S(Mdp.GetStateCount());
		//集計用変数((i,u)に遭遇した回数を集計)
		std::vector<std::vector<idx> > N(Mdp.GetStateCount());
		//0で初期化する
		for(idx i=0;i<S.size();i++)
		{
			S[i].resize(Mdp.GetControlCount(i),0.0);
			N[i].resize(Mdp.GetControlCount(i),0);
		}

		//Episodeの実行（最大episodecount回）
		for(idx k=0;k<episodecount;k++)
		{
			Episode e;
			bool result;
			result=Mdp.GetEpisode(e);
			//エピソード取得不可の場合は終了
			if(result==false)
			{
				break;
			}
			//コストの集計
			real t=0.0;
			for(idx m=e.GetStepCount()-1;m>=0;m--)
			{
				t=e[m].Cost+Mdp.GetDiscountRate()*t;
				S[e[m].State][e[m].Control]+=t;
				N[e[m].State][e[m].Control]++;
				if(m==0)
				{
					break;
				}
			}

		}

		value.resize(Mdp.GetStateCount());
		//価値観数の近似値の算出
		for(idx i=0;i<Mdp.GetStateCount();i++)
		{
			value[i].resize(Mdp.GetControlCount(i));

			for(idx u=0;u<Mdp.GetControlCount(i);u++)
			{
				if(N[i][u]!=0)
				{
					value[i][u]=S[i][u]/((real)N[i][u]);
				}
				else
				{
					value[i][u]=Mdp.GetCostMax();
				}
			}
		}
		return value;
	}
	StochasticPolicy& EpsilonGreedy(const Policy& p,real e,StochasticPolicy& out)const
	{
		//policy pと同値なStochastic Policyを作成
		Mdp.GetStochasticPolicy(p,out);

		//p[]に対するe-greedyポリシーを作成する
		for(idx i=0;i<out.GetStateCount();i++)
		{
			const idx controlcount=out[i].GetValueMax();
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
			out.SetProbability(i,prob);
		}

		return out;

	}
	void SetCurrentPolicy(const Policy& p)
	{
		Mdp.SetCurrentPolicy(p);
	}
	void SetCurrentPolicy(const StochasticPolicy& sp)
	{
		Mdp.SetCurrentPolicy(sp);
	}
};




}



#endif /* EPSILONSOFTONPOLICYMONTECARLO_H_ */
