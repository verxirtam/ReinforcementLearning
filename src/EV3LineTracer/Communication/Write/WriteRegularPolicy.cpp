/*
 * WriteRegularPolicy.cpp
 *
 *  Created on: 2015/08/16
 *      Author: daisuke
 */

#include "WriteRegularPolicy.h"



namespace RL
{

void WriteRegularPolicy::process(OutputContext& output)
{
	idx state_count = ev3LineTracer.GetStateCount();
	Policy p;
	ev3LineTracer.GetRegularPolicy(p);
	RL::WriteSinglePolicy wsp(p,0);
	for(idx i = 0; i < state_count; i++)
	{
		wsp.setStateIndex(i);
		wsp.process(output);
	}
}

} /* namespace RL */
