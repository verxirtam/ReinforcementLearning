/*
 * WriteRegularPolicy.cpp
 *
 *  Created on: 2015/08/21
 *      Author: daisuke
 */

#include "WriteRegularPolicy.h"



namespace RL
{
void WriteRegularPolicy::process(OutputContext& output)
{
	Policy rp = ev3LineTracer.getRegularPolicy();
	WritePolicy(rp).process(output);
}

} /* namespace RL */
