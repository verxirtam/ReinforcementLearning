/*
 * ReadPolicy.cpp
 *
 *  Created on: 2015/08/23
 *      Author: daisuke
 */

#include "ReadPolicy.h"



namespace RL
{
void ReadPolicy::process(InputContext &input)
{
	idx statecount = policy.getStateCount();

	ReadSinglePolicy rsp(policy);

	//pへのRegularPolicyの書き込み
	for(idx i=0; i<statecount; i++)
	{
		rsp.setStateIndex(i);
		rsp.process(input);
	}

}
} /* namespace RL */
