/*
 * Policy.h
 *
 *  Created on: 2013/12/23
 *      Author: daisuke
 */

#ifndef POLICY_H_
#define POLICY_H_

#include <vector>
#include <stdexcept>
#include "RLUtility.h"
#include "Random.h"


namespace RL
{


/*////////////////////////////////////////////////
 class Policy
 ポリシーを表すクラス
 内部にvector<index>としてStateごとのControlの値を格納している。
 コンストラクタで与えたvectorのサイズは後から変更できない。
 */////////////////////////////////////////////////
class Policy
{
private:
	std::vector<idx> Control;
public:
	//コンストラクタ
	//statecountで与えたサイズでvectorを初期化する
	inline Policy(idx statecount = 0)
		:Control(statecount,0)
	{
	}
	//デストラクタ
	//データメンバはSTLコンテナの実体のみのため何もしない
	virtual ~Policy()
	{
	}
	//添字演算子
	//vector<index>::operator[]()を利用するため範囲チェックはされないので注意
	inline idx operator[](idx i)const
	{
		return Control[i];
	}
	//Stateの個数(=vectorのサイズ)を返す
	inline idx GetStateCount()const
	{
		return (idx)Control.size();
	}
	//Policyを設定する
	//範囲外の場合はvector::at()によって例外std::out_of_rangeが投げられる
	void SetPolicy(idx i,idx c)
	{
		Control.at(i)=c;
	}
	//MDPとの整合性を確認する
	//不正な場合は例外std::range_errorが発生する
	template<typename MDP>
	void Verify(const MDP& mdp)const
	{
		//statecountが一致するかの確認
		if(this->GetStateCount()!=mdp.GetStateCount())
		{
			throw std::range_error("error at Policy::Verify().");
		}
		//policyの値がcontrolの個数を上回っていないこと
		for(idx i=0;i<this->GetStateCount();i++)
		{
			if((*this)[i]>=mdp.GetControlCount(i))
			{
				throw std::range_error("error at Policy::Verify().");
			}
		}
	}
	//等号演算子
	inline bool operator==(const Policy& org)const
	{
		return (this->Control)==(org.Control);
	}
	//不等号演算子
	inline bool operator!=(const Policy& org)const
	{
		return !((*this) == org);
	}
	//コピーコンストラクタ:動的データメンバなしのため定義不要
	//コピー代入演算子:動的データメンバなしのため定義不要

};

/*////////////////////////////////////////////////
 class StochasticPolicy
 確率的ポリシーを表すクラス
 内部にstd::vector<std::vector<real> >としてStateごとに、各Controlの選択確率を格納している。
 コンストラクタで与えたvectorのサイズは後から変更できない。
 */////////////////////////////////////////////////
typedef std::vector<RandomIdx > StochasticPolicyData;
class StochasticPolicy
{
private:
	StochasticPolicyData Control;
public:

	//コンストラクタ 引数：countrolcountのvector (const vector<idx>&)
	StochasticPolicy(const std::vector<idx>& controlcount);

	//デフォルトコンストラクタ
	//Controlは空のvectorとするので何もしない
	StochasticPolicy()
	{
	}

	//コンストラクタ 引数：countrolのvector (const StochasticPolicyData&)
	StochasticPolicy(const StochasticPolicyData& control)
	{
		Control=control;
		Correct();
	}

	//添字演算子
	inline const RandomIdx& operator[](idx i)const
	{
		return Control[i];
	}


	//状態数を取得する
	inline idx GetStateCount()const
	{
		return Control.size();
	}
	inline idx GetControlCount(idx i)const
	{
		return (idx)Control[i].GetValueMax();
	}
	//等号演算子
	inline bool operator==(const StochasticPolicy& org)const
	{
		return (this->Control)==(org.Control);
	}
	//不等号演算子
	inline bool operator!=(const StochasticPolicy& org)const
	{
		return !((*this) == org);
	}
	//stateに対する発生確率probを設定する
	void SetProbability(idx state,const std::vector<real>& prob);
	//ポリシーが選択確率として整合性が取れているかチェックする
	void Correct(idx state);
	//ポリシーが選択確率として整合性が取れているかチェックする
	void Correct();
	//MDPとの整合性を確認する
	//不正な場合は例外std::range_errorが発生する
	template<typename MDP>
	void Verify(const MDP& mdp)const
	{
		//statecountが一致するかの確認
		if(this->GetStateCount()!=mdp.GetStateCount())
		{
			throw std::range_error("error at StochasticPolicy::Verify().");
		}
		//選択されるcontrolの個数が一致していること
		for(idx i=0;i<this->GetStateCount();i++)
		{
			if((*this)[i].GetValueMax() != mdp.GetControlCount(i))
			{
				throw std::range_error("error at StochasticPolicy::Verify().");
			}
		}
	}

	//コピーコンストラクタ:動的データメンバなしのため定義不要
	//コピー代入演算子:動的データメンバなしのため定義不要
};


} /* namespace RL */

#endif /* POLICY_H_ */
