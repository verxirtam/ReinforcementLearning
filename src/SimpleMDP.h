/*
 * SimpleMDP.h
 *
 *  Created on: 2014/01/05
 *      Author: daisuke
 */

#ifndef SIMPLEMDP_H_
#define SIMPLEMDP_H_

#include "RLUtility.h"
#include "Policy.h"
#include "Episode.h"
#include "Random.h"

namespace RL
{

//単純なMDP RLMethodの単体テスト用
class SimpleMDP
{
private:
	//状態数
	idx StateCount;
	//RegularPolicyがterminate stateに向かう推移確率
	real TransitionProb;
	//(state,control)に対する推移確率を格納するvector
	std::vector<std::vector<RandomIdx> > Prob;
	//現在設定されているPolicy;
	StochasticPolicy CurrentPolicy;
public:
	//コンストラクタ
	SimpleMDP(idx length=2);
	//状態数の取得
	inline idx GetStateCount()const
	{
		return StateCount;
	}
	//Control数の取得
	inline idx GetControlCount(idx state)const
	{
		return Prob[state].size();
	}
	//MDPに適合したポリシーの取得
	Policy& GetPolicy(Policy& out)const
	{
		out=Policy(this->GetStateCount());
		return out;
	}
	//MDPに適合した標準的なポリシーの取得
	Policy& GetRegularPolicy(Policy& out)const
	{
		out=Policy(this->GetStateCount());
		for(idx i=0;i<GetStateCount();i++)
		{
			out.SetPolicy(i,0);
		}
		return out;
	}
	/*
	StochasticPolicy& GetStochasticPolicy(int i=0,StochasticPolicy& out)const
	{
		Policy p;
		out=GetStochasticPolicy(this->GetRegularPolicy(p));
		return out;
	}
	*/
	StochasticPolicy& GetStochasticPolicy(const Policy& p,StochasticPolicy& out)const
	{
		p.Verify(*this);
		StochasticPolicyData data(p.GetStateCount());
		for(idx i=0;i<data.size();i++)
		{
			std::vector<real> prob(this->GetControlCount(i),0.0);
			prob[p[i]]=1.0;
			data[i]=RandomIdx(prob,16);
		}
		out=StochasticPolicy(data);

		return out;
	}
	//割引率の取得(1.0を返すのみ)
	inline real GetDiscountRate()const
	{
		return 1.0;
	}
	Step GetNextStep(const Step& current)const
	{
		Step next;
		//現在のstateとPolicyから次の状態を取得
		next.State=Prob[current.State][current.Control].Get();
		next.Control=CurrentPolicy[next.State].Get();
		next.Cost=1.0;
		if(next.State==0)
		{
			next.Cost=0.0;
		}
		return next;
	}
	bool GetEpisode(Episode& episode)const
	{
		//返り値となるepisode
		episode=Episode();
		Step current;
		//最初のstate
		current.State=GetStateCount()-1;
		//Policyで決まるcontrolを取得
		current.Control=CurrentPolicy[current.State].Get();
		current.Cost=1.0;

		episode.AddStep(current);

		//terminate state(0)に達しない
		//かつ
		//ステップ数がCostMaxに達しない
		//の間、stepを追加する
		idx costmax=(idx)this->GetCostMax();
		while((current.State!=0)&&(episode.GetStepCount()<costmax))
		{
			current=GetNextStep(current);
			episode.AddStep(current);
		}
		return true;
	}
	inline void SetCurrentPolicy(const Policy& p)
	{
		StochasticPolicy sp;
		this->SetCurrentPolicy(this->GetStochasticPolicy(p,sp));
	}

	inline void SetCurrentPolicy(const StochasticPolicy& p)
	{
		p.Verify(*this);
		this->CurrentPolicy=p;
	}
	inline real GetCostMax()const
	{
		return this->GetStateCount()*10.0;
	}
	std::vector<real>& GetStateValueFunction(std::vector<real>& J)const;
	std::vector<std::vector<real> >& GetControlValueFunction(std::vector<std::vector<real> >& Q)const;
	//PolicyIteration実行前の初期化
	//SimpleMDPでは何もしない
	void Init(){}
};







}

#endif /* SIMPLEMDP_H_ */
