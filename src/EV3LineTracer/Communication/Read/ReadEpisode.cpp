/*
 * ReadEpisode.cpp
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#include "ReadEpisode.h"


namespace RL
{
void ReadEpisode::process(InputContext &input)
{
	//ステップ数の読み込み
	idx step_count = StringToData::parseTo<idx>(input.nextToken());
	input.skipReturn();
	//エピソードの内容を空にする
	episode.clear();
	//エピソードの長さを確保する
	episode.setStepCount(step_count);
	//エピソードの読み込み
	RL::ReadStep read_step(episode,0);
	for(idx n = 0; n < step_count; n++)
	{
		read_step.setStepIndex(n);
		read_step.process(input);
	}
}
} /* namespace RL */
