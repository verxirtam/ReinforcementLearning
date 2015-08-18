/*
 * WriteInterval.cpp
 *
 *  Created on: 2015/08/11
 *      Author: daisuke
 */

#include "WriteInterval.h"

namespace RL
{

void WriteInterval::process(OutputContext& output)
{
	output.writeToken(std::to_string(this->ev3LineTracer.getInterval()));
	output.newLine();
}

} /* namespace RL */
