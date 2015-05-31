/*
 * ReadState.cpp
 *
 *  Created on: 2015/01/24
 *      Author: daisuke
 */

#include "ReadSingleState.h"

namespace RL
{

ReadSingleState::ReadSingleState(EV3LineTracer &ev3,idx i) :
		ev3LineTracer(ev3),
		stateIndex(i)
{
}

ReadSingleState::~ReadSingleState()
{
}

/*
 <SingleState>::=
 <StateIndex><tab>;StateIndex
 <RefMax><tab>;RefMax
 <ControlCount>;ControlCount
 */
void ReadSingleState::process(InputContext &input)
{
	idx stateindex;
	real refmax;
	idx controlcount;

	//State[stateIndex]を読み取りev3LineTracerに設定する
	stateindex = StringToData::parseTo<idx>(input.nextToken());
	//読み取ったstateindexとstateIndexが異なっていれば例外発生
	if(stateindex != stateIndex)
	{
		std::string msg("invalid stateIndex");
		throw std::ios_base::failure(msg);
	}
	refmax = StringToData::parseTo<real>(input.nextToken());
	controlcount = StringToData::parseTo<idx>(input.nextToken());
	input.skipReturn();

	ev3LineTracer.setState(stateindex,refmax,controlcount);

}

} /* namespace RL */
