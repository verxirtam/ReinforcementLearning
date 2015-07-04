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
	real motorspeedmin = ev3LineTracer.getMotorSpeedMin();
	real motorspeedmax = ev3LineTracer.getMotorSpeedMax();


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

	lmoterspeed = StringToData::parseTo<real>(input.nextToken());
	if ((lmoterspeed < motorspeedmin) || ((motorspeedmax < lmoterspeed)))
	{
		std::string msg("invalid lmoterspeed");
		throw std::ios_base::failure(msg);
	}

	rmoterspeed = StringToData::parseTo<real>(input.nextToken());
	if ((rmoterspeed < motorspeedmin) || ((motorspeedmax < rmoterspeed)))
	{
		std::string msg("invalid rmoterspeed");
		throw std::ios_base::failure(msg);
	}


		input.skipReturn();

	ev3LineTracer.setControl(stateindex,controlindex,lmoterspeed,rmoterspeed);
}

} /* namespace RL */
