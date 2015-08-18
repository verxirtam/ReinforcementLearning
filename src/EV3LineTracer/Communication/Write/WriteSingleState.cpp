/*
 * WriteSingleState.cpp
 *
 *  Created on: 2015/08/11
 *      Author: daisuke
 */

#include "WriteSingleState.h"

namespace RL
{

void WriteSingleState::process(OutputContext& output)
{
	output.writeToken(std::to_string(this->index));
	output.writeToken(std::to_string(this->ev3LineTracer.getState(index).refMax));
	output.writeToken(std::to_string(this->ev3LineTracer.getControlCount(index)));
	output.newLine();
}

} /* namespace RL */
