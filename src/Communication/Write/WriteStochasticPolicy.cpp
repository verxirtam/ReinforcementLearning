/*
 * WriteStochasticPolicy.cpp
 *
 *  Created on: 2015/08/25
 *      Author: daisuke
 */

#include "WriteStochasticPolicy.h"



namespace RL
{

void WriteStochasticPolicy::process(OutputContext& output)
{
	//StateCountの取得
	idx state_count=stochasticPolicy.getStateCount();

	//state毎の繰り返し
	for(idx i=0; i < state_count; i++)
	{
		//StateIndexの書き込み
		output.writeToken(std::to_string(i));
		//COntrolCountの取得
		idx control_count = stochasticPolicy.getControlCount(i);

		//controlについて繰り返し
		for(idx u=0; u < control_count; u++)
		{
			//controlを選択する確率を取得
			real prob_i_u = stochasticPolicy[i].getProbability(u);
			//取得した確率を書き込み
			output.writeToken(std::to_string(prob_i_u));
		}
		//改行を書き込み
		output.newLine();
	}
}


} /* namespace RL */
