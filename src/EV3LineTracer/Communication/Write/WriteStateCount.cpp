/*
 * WriteStateCount.cpp
 *
 *  Created on: 2015/08/11
 *      Author: daisuke
 */

#include "WriteStateCount.h"

namespace RL
{

void WriteStateCount::process(OutputContext& output)
{
	output.writeToken(std::to_string(this->ev3LineTracer.GetStateCount()));
	output.newLine();
}

} /* namespace RL */
