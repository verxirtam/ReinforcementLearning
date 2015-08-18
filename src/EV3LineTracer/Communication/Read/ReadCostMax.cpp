/*
 * ReadCostMax.cpp
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#include "ReadCostMax.h"


namespace RL
{
void ReadCostMax::process(InputContext &input)
{
	std::string token = input.nextToken();
	real cost_max = StringToData::parseTo<real>(token);
	ev3LineTracer.setCostMax(cost_max);
	input.skipReturn();
}

} /* namespace RL */
