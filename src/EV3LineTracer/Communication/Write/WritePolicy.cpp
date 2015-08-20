/*
 * WriteRegularPolicy.cpp
 *
 *  Created on: 2015/08/16
 *      Author: daisuke
 */

#include "WritePolicy.h"



namespace RL
{

void WritePolicy::process(OutputContext& output)
{
	idx state_count = policy.getStateCount();
	RL::WriteSinglePolicy wsp(policy,0);
	for(idx i = 0; i < state_count; i++)
	{
		wsp.setStateIndex(i);
		wsp.process(output);
	}
}

} /* namespace RL */
