/*
 * WriteEpisode.cpp
 *
 *  Created on: 2015/09/05
 *      Author: daisuke
 */

#include "WriteEpisode.h"
#include "WriteStep.h"

namespace RL
{
void WriteEpisode::process(OutputContext& output)
{
	//ステップ数の取得
	idx step_count = episode.getStepCount();
	//ステップ数の書き込み
	output.writeToken(std::to_string(step_count));

	RL::WriteStep ws(episode[0],0);
	//各ステップの書き込み
	for(idx n = 0; n < step_count; n++)
	{
		RL::WriteStep(episode[n],n).process(output);
	}
}

} /* namespace RL */
