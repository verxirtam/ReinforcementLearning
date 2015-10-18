/*
 * EpsilonSoftOnPolicyMonteCarlo.h
 *
 *  Created on: 2014/01/15
 *      Author: daisuke
 */

#ifndef EPSILONSOFTONPOLICYMONTECARLO_H_
#define EPSILONSOFTONPOLICYMONTECARLO_H_

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

#include "Communication/OutputPolicyEvaluationLogFile.h"
#include "Communication/TSVOutputContext.h"
#include "Episode.h"
#include "Policy.h"
#include "Random.h"
#include "RLUtility.h"
#include "TimeToString.h"

namespace RL
{
class PolicyEvaluationStatistics;
} /* namespace RL */


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
	//control-value functionを表すコンテナ
	typedef std::vector<std::vector<real> > ControlValueFunction;
	//処理対象のMDP
	MDP& mdp;
	//ポリシー以外のcontrolを選択する確率を示す
	//(PolicyIterationによって変化しない)
	real epsilon;
	//ログファイル出力を行うかどうかを表す
	bool loggingEnable;
	//ログファイルを格納するパス(末尾に'/'を付けること)
	std::string logDirectoryPath;
	//PolicyEvaluation()開始時刻
	std::time_t startPolicyEvaluationTime;
public:
	EpsilonSoftOnPolicyMonteCarlo(MDP& mdp_,double epsilon_ = 0.125, bool logging_enable = false,
			std::string log_directory_path = "") :
			mdp(mdp_),epsilon(epsilon_), loggingEnable(logging_enable),
			logDirectoryPath(log_directory_path),
			startPolicyEvaluationTime(0)
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

		//初期ポリシーの設定(RegularPolicy)
		mdp.getRegularPolicy(out);

		//Policy Iterationの実行
		for(idx k=0;k<pi_count;k++)
		{
			setCurrentPolicy(epsilonGreedy(out,epsilon,mu_e));
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
	ControlValueFunction& policyEvaluation(ControlValueFunction& value, idx episodecount=10000)
	{
		if(loggingEnable)
		{
			//現在時刻をstartPolicyEvaluationTimeに取得
			std::time(&startPolicyEvaluationTime);
		}
		//集計用変数((i,u)のコストを集計する)
		std::vector<std::vector<real> > S(mdp.getStateCount());
		//集計用変数((i,u)に遭遇した回数を集計)
		std::vector<std::vector<idx> > N(mdp.getStateCount());

		//ログ出力用変数
		//集計用変数((i,u)のコストの和の2乗を集計する)
		std::vector<std::vector<real> > SQ(0);
		//集計用変数((i,u)の選択回数の2乗を集計する)
		std::vector<std::vector<idx> > NQ(0);
		//集計用変数((i,u)を選択したエピソード数を集計する)
		std::vector<std::vector<idx> > K(0);
		//集計用変数(1エピソードあたりの(i,u)のコストを集計する)
		std::vector<std::vector<real> > Sk(0);
		//集計用変数(1エピソードあたりの(i,u)に遭遇した回数を集計)
		std::vector<std::vector<idx> > Nk(0);
		//集計用変数(1エピソードあたりの(i,u)のコストの2乗を集計する)
		std::vector<std::vector<real> > SQk(0);

		//ログ出力用変数の初期化(ログ出力設定時のみ実行)
		if(loggingEnable)
		{
			SQ.resize(mdp.getStateCount());
			NQ.resize(mdp.getStateCount());
			K.resize(mdp.getStateCount());
			Sk.resize(mdp.getStateCount());
			Nk.resize(mdp.getStateCount());
			SQk.resize(mdp.getStateCount());
		}

		//0で初期化する
		for(idx i=0;i<S.size();i++)
		{
			S[i].resize(mdp.getControlCount(i),0.0);
			N[i].resize(mdp.getControlCount(i),0);
			if(loggingEnable)
			{
				SQ[i].resize(mdp.getControlCount(i),0.0);
				NQ[i].resize(mdp.getControlCount(i),0.0);
				K[i].resize(mdp.getControlCount(i),0);
			}
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
			//エピソードごとの初期化
			if(loggingEnable)
			{
				for(idx i=0;i<S.size();i++)
				{
					Sk[i]=std::vector<real>(mdp.getControlCount(i),0.0);
					Nk[i]=std::vector<idx>(mdp.getControlCount(i),0);
					SQk[i]=std::vector<real>(mdp.getControlCount(i),0.0);
				}
			}

			//コストの集計
			real t=0.0;
			idx step_count = e.getStepCount();
			//Step単位の集計
			for(idx m = step_count -1; m >= 0; m--)
			{
				t=e[m].cost+mdp.getDiscountRate()*t;
				S[e[m].state][e[m].control]+=t;
				N[e[m].state][e[m].control]++;
				//ログ出力用変数の算出
				if(loggingEnable)
				{
					//SQ[e[m].state][e[m].control]+=t*t;
					Sk[e[m].state][e[m].control]+=t;
					Nk[e[m].state][e[m].control]++;
					SQk[e[m].state][e[m].control]+=t*t;
				}
				if(m==0)
				{
					break;
				}
			}
			//Episode単位の集計
			if(loggingEnable)
			{
				for(idx i=0;i<S.size();i++)
				{
					idx control_count=mdp.getControlCount(i);
					for(idx u=0;u<control_count;u++)
					{
						SQ[i][u] += Sk[i][u]*Sk[i][u];
						//SQ[i][u]の集計
						//NQ[i][u]の集計
						NQ[i][u] += Nk[i][u]*Nk[i][u];
						//K[i][u]を集計する
						//エピソード中に1回以上(i,u)を選択していたらK[i][u]を1加える
						if(Nk[i][u]>=1)
						{
							K[i][u]++;
						}
					}
				}
			}
			//ログファイルへの書き込み
			if(loggingEnable)
			{
				PolicyEvaluationStatistics pes(Sk,Nk,SQk,S,N,K,SQ,NQ);
				writePolicyEvaluationLogfile(pes,k);
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
	void writeFile(const std::string& logfilepath,
			RL::OutputProcedure& output)const
	{
		std::ofstream ofs(logfilepath);
		//ファイルオープンの確認
		if (!ofs)
		{
			std::string msg("writeFile() : can not open ");
			msg += logfilepath;
			throw std::ios_base::failure(msg);
		}
		try
		{
			RL::TSVOutputContext toc(ofs);
			output.process(toc);
			ofs.close();
		} catch (...)
		{
			ofs.close();
			throw;
		}
	}
	void writePolicyEvaluationLogfile(RL::PolicyEvaluationStatistics& pes,idx episodeIndex)const
	{
		RL::OutputPolicyEvaluationLogFile ope(pes,episodeIndex,epsilon);

		std::stringstream log_file_path("");

		log_file_path << this->logDirectoryPath;
		log_file_path << "EpsilonSoftOnPolicyMonteCarlo";
		log_file_path << "_";
		log_file_path << (typeid (&mdp)).name();
		log_file_path << "_";
		log_file_path << "PolicyEvaluation";
		log_file_path << "_";
		log_file_path << RL::TimeToString::toStringForFileName(startPolicyEvaluationTime);
		log_file_path << "_";
		log_file_path << "Episode";
		log_file_path << "_";
		log_file_path << std::setw(3) << std::setfill('0') << std::to_string(episodeIndex);
		log_file_path << "_";
		log_file_path << ".log";

		writeFile(log_file_path.str(),ope);
	}
};



}



#endif /* EPSILONSOFTONPOLICYMONTECARLO_H_ */
