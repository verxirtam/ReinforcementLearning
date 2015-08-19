/*
 * OutputCommandExecEpisode.cpp
 *
 *  Created on: 2015/08/20
 *      Author: daisuke
 */

#include "OutputCommandExecEpisode.h"


namespace RL
{
void OutputCommandExecEpisode::process(OutputContext& output)
{
	output.writeToken("ExecEpisode");
	output.newLine();
}
} /* namespace RL */
