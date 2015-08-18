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
	control(controlcount.size())
{
	idx i;
	for(i=0;i<control.size();i++)
	{
		if(controlcount[i]<=0)
		{
			throw std::invalid_argument("error on StochasticPolicy::StochasticPolicy().");
		}
		vector<real> r(controlcount[i],0.0);
		r[0]=1.0;
		control[i]=RandomIdx(r,16);
	}
	this->correct();
}

//stateに対する発生確率probを設定する
//整合性チェックCorrect()も実行される
void StochasticPolicy::setProbability(idx state,const std::vector<real>& prob)
{
	//引数のvectorの長さをチェック
	if(control[state].getValueMax()!=prob.size())
	{
		//引数のvectorの長さControl[state]と異なる場合は
		//out_of_range例外を投げる
		throw std::out_of_range("error on StochasticPolicy::SetProbability().");
	}
	//引数のvectorの長さControl[state]と同じなら
	//probをControl[state]にコピーする
	control[state]=RandomIdx(prob,16);
	//整合性チェック(NGなら例外発生)
	correct(state);
}
//ポリシーが選択確率として整合性が取れるように修正する
//修正できない場合は例外を投げる
void StochasticPolicy::correct(idx state)
{
	if((state==0) && (control[state].getValueMax()!=1))
	{
		throw std::out_of_range("error on StochasticPolicy::Correct().");
	}

}
//ポリシーが選択確率として整合性が取れるように修正する
//修正できない場合は例外を投げる
void StochasticPolicy::correct()
{
	for(idx i=0;i<control.size();i++)
	{
		correct(i);
	}
}



} /* namespace RL */
