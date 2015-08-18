/*
 * WriteCostMax.cpp
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#include "WriteCostMax.h"



namespace RL
{

void WriteCostMax::process(OutputContext& output)
{
	output.writeToken(std::to_string(this->ev3LineTracer.getCostMax()));
	output.newLine();
}


} /* namespace RL */
