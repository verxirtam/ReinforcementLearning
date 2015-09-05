/*
 * OutputEV3LineTracerSettingFile.cpp
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#include "OutputEV3LineTracerSettingFile.h"


namespace RL
{
void OutputEV3LineTracerSettingFile::process(OutputContext& output)
{
	std::string construct_time = ev3LineTracer.getConstructTimeString();
	output.writeToken(construct_time);
	output.newLine();

	WriteInterval(ev3LineTracer).process(output);

	WriteCostMax(ev3LineTracer).process(output);

	WriteStateCount(ev3LineTracer).process(output);

	WriteState(ev3LineTracer).process(output);

	WriteControl(ev3LineTracer).process(output);

	WriteRegularPolicy<EV3LineTracer>(ev3LineTracer).process(output);
}

} /* namespace RL */
