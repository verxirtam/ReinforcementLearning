/*
 * ReadState.cpp
 *
 *  Created on: 2015/05/31
 *      Author: daisuke
 */

#include "ReadState.h"



namespace RL
{

ReadState::ReadState(EV3LineTracer &ev3):
		ev3LineTracer(ev3)
{

}

ReadState::~ReadState()
{
	// TODO Auto-generated destructor stub
}
void ReadState::process(InputContext &input)
{
	idx statecount = ev3LineTracer.GetStateCount();

	ReadSingleState rss(ev3LineTracer);

	for(idx i=0; i<statecount; i++)
	{
		rss.SetStateIndex(i);
		rss.process(input);
	}
}

} /* namespace RL */
