/*
 * ReadStep.cpp
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#include "ReadStep.h"




namespace RL
{
void ReadStep::process(InputContext &input)
{
	//episode indexの確認
	idx step_index = StringToData::parseTo<idx>(input.nextToken());
	if(stepIndex != step_index)
	{
		throw std::ios_base::failure("episode index is failure.\n");
	}
	//stepの取得
	idx state = StringToData::parseTo<idx>(input.nextToken());
	idx control = StringToData::parseTo<idx>(input.nextToken());
	real cost = StringToData::parseTo<real>(input.nextToken());
	input.skipReturn();

	//stepの書き込み
	step=Step(state,control,cost);

}
} /* namespace RL */
