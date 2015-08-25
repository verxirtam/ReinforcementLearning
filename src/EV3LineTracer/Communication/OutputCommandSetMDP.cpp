/*
 * OutputCommandSetMDP.cpp
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#include "OutputCommandSetMDP.h"



namespace RL
{
void OutputCommandSetMDP::process(OutputContext& output)
{
	output.writeToken("SetMDP");
	output.newLine();

	WriteInterval(ev3LineTracer).process(output);

	WriteCostMax(ev3LineTracer).process(output);

	WriteStateCount(ev3LineTracer).process(output);

	WriteState(ev3LineTracer).process(output);

	WriteControl(ev3LineTracer).process(output);

	WriteRegularPolicy<EV3LineTracer>(ev3LineTracer).process(output);
}
} /* namespace RL */
