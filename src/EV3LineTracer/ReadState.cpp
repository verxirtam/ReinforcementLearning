/*
 * ReadState.cpp
 *
 *  Created on: 2015/01/24
 *      Author: daisuke
 */

#include "ReadState.h"

namespace RL
{

ReadState::ReadState(EV3LineTracer &ev3) :
		ev3LineTracer(ev3)
{
}

ReadState::~ReadState()
{
}

/*
 <SingleState>::=
 <StateIndex><tab>;StateIndex
 <RefMax><tab>;RefMax
 <ControlCount>;ControlCount
 */
void ReadState::process(InputContext &input)
{
	idx stateindex;
	real refmax;
	idx controlcount;
	stateindex = StringToData::parseTo<idx>(input.nextToken());
	refmax = StringToData::parseTo<real>(input.nextToken());
	controlcount =  StringToData::parseTo<idx>(input.nextToken());
	input.skipReturn();

	ev3LineTracer.setState(stateindex,refmax,controlcount);
}

} /* namespace RL */
