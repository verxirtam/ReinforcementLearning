/*
 * ReadInterval.cpp
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#include "ReadInterval.h"

using namespace std;

namespace RL
{

ReadInterval::ReadInterval(EV3LineTracer &ev3):ev3LineTracer(ev3)
{
}

ReadInterval::~ReadInterval()
{
}
void ReadInterval::process(InputContext &input)
{
	string token = input.nextToken();
	idx interval = StringToData::parseTo<idx>(token);
	ev3LineTracer.setInterval(interval);
	input.skipReturn();
}

} /* namespace RL */
