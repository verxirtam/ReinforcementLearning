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
	idx stateCount;
	//RegularPolicyがterminate stateに向かう推移確率
	real transitionProb;
	//(state,control)に対する推移確率を格納するvector
	std::vector<std::vector<RandomIdx> > prob;
	//現在設定されているPolicy;
	StochasticPolicy currentPolicy;
public:
	//コンストラクタ
	SimpleMDP(idx length=2);
	//状態数の取得
	inline idx getStateCount()const
	{
		return stateCount;
	}
	//Control数の取得
	inline idx getControlCount(idx state)const
	{
		return prob[state].size();
	}
	//MDPに適合したポリシーの取得
	Policy& getPolicy(Policy& out)const
	{
		out=Policy(this->getStateCount());
		return out;
	}
	//MDPに適合した標準的なポリシーの取得
	Policy& getRegularPolicy(Policy& out)const
	{
		out=Policy(this->getStateCount());
		for(idx i=0;i<getStateCount();i++)
		{
			out.setPolicy(i,0);
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
	StochasticPolicy& getStochasticPolicy(const Policy& p,StochasticPolicy& out)const
	{
		p.verify(*this);
		StochasticPolicyData data(p.getStateCount());
		for(idx i=0;i<data.size();i++)
		{
			std::vector<real> prob(this->getControlCount(i),0.0);
			prob[p[i]]=1.0;
			data[i]=RandomIdx(prob,16);
		}
		out=StochasticPolicy(data);

		return out;
	}

	StochasticPolicy& getCurrentPolicy(StochasticPolicy& out)const
	{
		out = StochasticPolicy(this->currentPolicy);
		return out;
	}

	StochasticPolicy getCurrentPolicy()const
	{
		return this->currentPolicy;
	}
	//割引率の取得(1.0を返すのみ)
	inline real getDiscountRate()const
	{
		return 1.0;
	}
	Step getNextStep(const Step& current)const
	{
		Step next;
		//現在のstateとPolicyから次の状態を取得
		next.state=prob[current.state][current.control].get();
		next.control=currentPolicy[next.state].get();
		next.cost=1.0;
		if(next.state==0)
		{
			next.cost=0.0;
		}
		return next;
	}
	bool getEpisode(Episode& episode)const
	{
		//返り値となるepisode
		episode=Episode();
		Step current;
		//最初のstate
		current.state=getStateCount()-1;
		//Policyで決まるcontrolを取得
		current.control=currentPolicy[current.state].get();
		current.cost=1.0;

		episode.addStep(current);

		//terminate state(0)に達しない
		//かつ
		//ステップ数がCostMaxに達しない
		//の間、stepを追加する
		idx costmax=(idx)this->getCostMax();
		while((current.state!=0)&&(episode.getStepCount()<costmax))
		{
			current=getNextStep(current);
			episode.addStep(current);
		}
		return true;
	}
	inline void setCurrentPolicy(const Policy& p)
	{
		StochasticPolicy sp;
		this->setCurrentPolicy(this->getStochasticPolicy(p,sp));
	}

	inline void setCurrentPolicy(const StochasticPolicy& p)
	{
		p.verify(*this);
		this->currentPolicy=p;
	}
	inline real getCostMax()const
	{
		return this->getStateCount()*10.0;
	}
	std::vector<real>& getStateValueFunction(std::vector<real>& J)const;
	std::vector<std::vector<real> >& getControlValueFunction(std::vector<std::vector<real> >& Q)const;
	//PolicyIteration実行前の初期化
	//SimpleMDPでは何もしない
	void init(){}
};







}

#endif /* SIMPLEMDP_H_ */
