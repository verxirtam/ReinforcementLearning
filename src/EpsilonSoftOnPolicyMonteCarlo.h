/*
 * EpsilonSoftOnPolicyMonteCarlo.h
 *
 *  Created on: 2014/01/15
 *      Author: daisuke
 */

#ifndef EPSILONSOFTONPOLICYMONTECARLO_H_
#define EPSILONSOFTONPOLICYMONTECARLO_H_

#include <vector>

#include "Episode.h"
#include "Policy.h"
#include "Random.h"
#include "RLUtility.h"


namespace RL
{

class PolicyEvaluationStatistics
{
public:
	typedef std::vector<std::vector<real> > TotalControlCostType;
	typedef std::vector<std::vector< idx> > TotalControlCountType;

	const TotalControlCostType& totalControlCostAtEpisode;//Sk;//Episode中のcontrolのcostの合計
	const TotalControlCountType& totalControlSelectCountAtEpisode;//Nk;//Episode中のcontrolを選択した回数
	const TotalControlCostType& totalControlCostSqureAtEpisode;//SQk;//Episode中のcontrolのcost^2の合計
	const TotalControlCostType& totalControlCost;//S;//controlのcostの合計
	const TotalControlCountType& totalControlSelectCount;//N;//controlを選択した回数
	const TotalControlCostType& totalControlCostSqure;//SQ;//controlのcost^2の合計

	PolicyEvaluationStatistics(
			const TotalControlCostType& totalCostOfControlAtEpisode_,//Sk;//Episode中のcontrolのcostの合計
			const TotalControlCountType& totalControlSelectCountAtEpisode_,//Nk;//Episode中のcontrolを選択した回数
			const TotalControlCostType& totalControlCostSqureAtEpisode_,//SQk;//Episode中のcontrolのcost^2の合計
			const TotalControlCostType& totalControlCost_,//S;//controlのcostの合計
			const TotalControlCountType& totalControlSelectCount_,//N;//controlを選択した回数
			const TotalControlCostType& totalControlCostSqure_//SQ;//controlのcost^2の合計
	):
		totalControlCostAtEpisode(totalCostOfControlAtEpisode_),
		totalControlSelectCountAtEpisode(totalControlSelectCountAtEpisode_),
		totalControlCostSqureAtEpisode(totalControlCostSqureAtEpisode_),
		totalControlCost(totalControlCost_),
		totalControlSelectCount(totalControlSelectCount_),
		totalControlCostSqure(totalControlCostSqure_)
	{
	}
};




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
	//control-value functionを表すコンテナ
	typedef std::vector<std::vector<real> > ControlValueFunction;
	//処理対象のMDP
	MDP& mdp;
public:
	EpsilonSoftOnPolicyMonteCarlo(MDP& mdp_):mdp(mdp_)
	{
	}
	//CurrentPolicyを
	//control-value functionに基づいて改善したPolicyを求める
	Policy& policyImprovement(const ControlValueFunction& Q,Policy& out)const
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
			ControlValueFunction Q;
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
	ControlValueFunction& policyEvaluation(ControlValueFunction& value, idx episodecount=10000)const
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
			idx step_count = e.getStepCount();
			for(idx m = step_count -1; m >= 0; m--)
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

		idx state_count = mdp.getStateCount();
		value.resize(state_count);
		//価値観数の近似値の算出
		for(idx i=0; i < state_count ;i++)
		{
			idx control_count = mdp.getControlCount(i);
			value[i].resize(control_count);

			for(idx u = 0; u < control_count; u++)
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

		idx state_count = out.getStateCount();
		//p[]に対するe-greedyポリシーを作成する
		for(idx i=0;i<state_count;i++)
		{
			const idx control_count=out[i].getValueMax();
			std::vector<real> prob(control_count, 0.0);
			for(idx u=0;u<control_count;u++)
			{
				if(u==p[i])
				{
					prob[u]=1.0 - e + e/((real)control_count);
				}
				else
				{
					prob[u]=e/((real)control_count);
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
