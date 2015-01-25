/*
 * ReadControl.cpp
 *
 *  Created on: 2015/01/25
 *      Author: daisuke
 */

#include "ReadControl.h"

namespace RL
{

ReadControl::ReadControl(EV3LineTracer &ev3):ev3LineTracer(ev3)
{
}

ReadControl::~ReadControl()
{
}
void ReadControl::process(InputContext &input)
{
	//Control[stateindex][controlindex]を読み取りev3LineTracerに設定する
	idx stateindex;
	idx controlindex;
	real lmoterspeed;
	real rmoterspeed;
	stateindex = StringToData::parseTo<idx>(input.nextToken());
	controlindex = StringToData::parseTo<idx>(input.nextToken());
	lmoterspeed = StringToData::parseTo<idx>(input.nextToken());
	rmoterspeed = StringToData::parseTo<idx>(input.nextToken());
	input.skipReturn();

	ev3LineTracer.setControl(stateindex,controlindex,lmoterspeed,rmoterspeed);
}

} /* namespace RL */
