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
	//読み取り用Stepの初期化
	Step step;
	ReadStep rs(step,0);
	//エピソードの読み込み
	for(idx n = 0; n < step_count; n++)
	{
		rs.setStepIndex(n);
		rs.process(input);
		episode.setStep(n,step);
	}
}
} /* namespace RL */
