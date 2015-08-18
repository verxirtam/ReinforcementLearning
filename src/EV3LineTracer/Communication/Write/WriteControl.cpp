/*
 * WriteControl.cpp
 *
 *  Created on: 2015/08/15
 *      Author: daisuke
 */

#include "WriteControl.h"



namespace RL
{
void WriteControl::process(OutputContext& output)
{
	idx state_count = ev3LineTracer.getStateCount();
	RL::WriteSingleControl wsc(ev3LineTracer,0,0);
	for(idx i = 0; i < state_count; i++)
	{
		wsc.setStateIndex(i);
		idx control_count = ev3LineTracer.getControlCount(i);
		for(idx u = 0; u < control_count; u++)
		{
			wsc.setControlIndex(u);
			wsc.process(output);
		}
	}
}

} /* namespace RL */
