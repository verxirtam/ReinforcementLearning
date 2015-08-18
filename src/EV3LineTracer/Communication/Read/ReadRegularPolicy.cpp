/*
 * ReadRegularPolicy.cpp
 *
 *  Created on: 2015/05/31
 *      Author: daisuke
 */

#include "ReadRegularPolicy.h"




namespace RL
{

ReadRegularPolicy::ReadRegularPolicy(EV3LineTracer& ev3) : ev3LineTracer(ev3)
{

}

ReadRegularPolicy::~ReadRegularPolicy()
{
	// TODO Auto-generated destructor stub
}
void ReadRegularPolicy::process(InputContext &input)
{
	idx statecount = ev3LineTracer.getStateCount();
	Policy p(statecount);

	ReadSinglePolicy rsp(ev3LineTracer,p);

	//pへのRegularPolicyの書き込み
	for(idx i=0; i<statecount; i++)
	{
		rsp.setStateIndex(i);
		rsp.process(input);
	}

	//ev3LineTracer.RegularPolicyへの書き込み
	ev3LineTracer.setRegularPolicy(p);

}
} /* namespace RL */
