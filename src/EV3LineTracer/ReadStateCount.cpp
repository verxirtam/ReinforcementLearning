/*
 * ReadStateCount.cpp
 *
 *  Created on: 2015/01/12
 *      Author: daisuke
 */

#include "ReadStateCount.h"

namespace RL
{

using namespace std;

ReadStateCount::ReadStateCount(EV3LineTracer &ev3):ev3LineTracer(ev3)
{
}

ReadStateCount::~ReadStateCount()
{
}

void ReadStateCount::process(InputContext &input)
{
	string token = input.nextToken();
	idx state_count = StringToData::parseTo<idx>(token);
	ev3LineTracer.setStateCount(state_count);
	input.skipReturn();
}

} /* namespace RL */
