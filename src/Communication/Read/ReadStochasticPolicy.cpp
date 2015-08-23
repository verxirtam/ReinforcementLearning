/*
 * ReadStochasticPolicy.cpp
 *
 *  Created on: 2015/08/23
 *      Author: daisuke
 */

#include "ReadStochasticPolicy.h"




namespace RL
{
void ReadStochasticPolicy::process(InputContext &input)
{
	//state数の読み込み
	idx state_count = stochasticPolicy.getStateCount();
	StochasticPolicyData policy_data(state_count);
	for(idx i=0; i < state_count; i++)
	{
		//stateindexを確認する
		input.skipToken(std::to_string(i));
		//state i における推移確率を読み込む
		//control数の読み込み
		idx control_count = stochasticPolicy[i].getValueMax();
		//推移確率取得用のvectorの初期化
		std::vector<real> prob(control_count, 0.0);
		for(idx u=0; u < control_count; u++)
		{
			prob[u] = StringToData::parseTo<real>(input.nextToken());
		}
		//policy_dataに推移確率を設定
		policy_data[i]=RandomIdx(prob);
		input.skipReturn();
	}
	//読み込み用ポリシーに設定
	stochasticPolicy = StochasticPolicy(policy_data);
}
} /* namespace RL */
