/*
 * OutputCommandSetCurrentPolicy.cpp
 *
 *  Created on: 2015/08/30
 *      Author: daisuke
 */

#include "OutputCommandSetCurrentPolicy.h"


namespace RL
{
void OutputCommandSetCurrentPolicy::process(OutputContext& output)
{
	output.writeToken("SetCurrentPolicy");
	output.newLine();
	WriteCurrentPolicy<EV3LineTracer>(ev3LineTracer).process(output);
}

} /* namespace RL */
