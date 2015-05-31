/*
 * ReadControl.cpp
 *
 *  Created on: 2015/01/25
 *      Author: daisuke
 */

#include "ReadSingleControl.h"

namespace RL
{

ReadSingleControl::ReadSingleControl(EV3LineTracer &ev3,idx si, idx ci) :
		ev3LineTracer(ev3)
{
		setIndex(si, ci);
}

ReadSingleControl::~ReadSingleControl()
{
}
void ReadSingleControl::process(InputContext &input)
{
	//Control[stateindex][controlindex]を読み取りev3LineTracerに設定する
	idx stateindex;
	idx controlindex;
	real lmoterspeed;
	real rmoterspeed;

	stateindex = StringToData::parseTo<idx>(input.nextToken());
	//読み取ったstateindexとstateIndexが異なっていれば例外発生
	if(stateindex != stateIndex)
	{
		std::string msg("invalid stateIndex");
		throw std::ios_base::failure(msg);
	}

	controlindex = StringToData::parseTo<idx>(input.nextToken());
	//読み取ったcontrolindexとcontrolIndexが異なっていれば例外発生
	if(controlindex != controlIndex)
	{
		std::string msg("invalid controlIndex");
		throw std::ios_base::failure(msg);
	}

	lmoterspeed = StringToData::parseTo<idx>(input.nextToken());
	rmoterspeed = StringToData::parseTo<idx>(input.nextToken());
	input.skipReturn();

	ev3LineTracer.setControl(stateindex,controlindex,lmoterspeed,rmoterspeed);
}

} /* namespace RL */
