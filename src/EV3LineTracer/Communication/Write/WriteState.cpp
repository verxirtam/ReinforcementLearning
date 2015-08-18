/*
 * WriteState.cpp
 *
 *  Created on: 2015/08/12
 *      Author: daisuke
 */

#include "WriteState.h"


namespace RL
{


void WriteState::process(OutputContext& output)
{
	idx state_count=ev3LineTracer.getStateCount();
	WriteSingleState wss(ev3LineTracer,0);

	for(idx i=0;i<state_count;i++)
	{
		wss.setStateIndex(i);
		wss.process(output);
	}
}
} /* namespace RL */
