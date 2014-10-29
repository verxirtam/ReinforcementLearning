/*
 * Policy.cpp
 *
 *  Created on: 2013/12/23
 *      Author: daisuke
 */

#include "Policy.h"


using namespace std;


namespace RL
{



//コンストラクタ 引数：countrolcountのvector (const vector<idx>&)
StochasticPolicy::StochasticPolicy(const std::vector<idx>& controlcount):
	Control(controlcount.size())
{
	idx i;
	for(i=0;i<Control.size();i++)
	{
		if(controlcount[i]<=0)
		{
			throw std::invalid_argument("error on StochasticPolicy::StochasticPolicy().");
		}
		vector<real> r(controlcount[i],0.0);
		r[0]=1.0;
		Control[i]=RandomIdx(r,16);
	}
	this->Correct();
}

//stateに対する発生確率probを設定する
//整合性チェックCorrect()も実行される
void StochasticPolicy::SetProbability(idx state,const std::vector<real>& prob)
{
	//引数のvectorの長さをチェック
	if(Control[state].GetValueMax()!=prob.size())
	{
		//引数のvectorの長さControl[state]と異なる場合は
		//out_of_range例外を投げる
		throw std::out_of_range("error on StochasticPolicy::SetProbability().");
	}
	//引数のvectorの長さControl[state]と同じなら
	//probをControl[state]にコピーする
	Control[state]=RandomIdx(prob,16);
	//整合性チェック(NGなら例外発生)
	Correct(state);
}
//ポリシーが選択確率として整合性が取れるように修正する
//修正できない場合は例外を投げる
void StochasticPolicy::Correct(idx state)
{
	if((state==0) && (Control[state].GetValueMax()!=1))
	{
		throw std::out_of_range("error on StochasticPolicy::Correct().");
	}

}
//ポリシーが選択確率として整合性が取れるように修正する
//修正できない場合は例外を投げる
void StochasticPolicy::Correct()
{
	for(idx i=0;i<Control.size();i++)
	{
		Correct(i);
	}
}



} /* namespace RL */
