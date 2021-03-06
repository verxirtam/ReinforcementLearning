/*
 * WriteSingleControl.cpp
 *
 *  Created on: 2015/08/12
 *      Author: daisuke
 */

#include "WriteSingleControl.h"

namespace RL
{
void WriteSingleControl::process(OutputContext& output)
{
	EV3LineTracerControl control = ev3LineTracer.getControl(stateIndex,controlIndex);

	output.writeToken(std::to_string(stateIndex));
	output.writeToken(std::to_string(controlIndex));
	output.writeToken(std::to_string(control.lMotorSpeed));
	output.writeToken(std::to_string(control.rMotorSpeed));
	output.newLine();


}


} /* namespace RL */
