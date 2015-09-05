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

	WriteEV3LineTracerSetting(ev3LineTracer).process(output);
}
} /* namespace RL */
