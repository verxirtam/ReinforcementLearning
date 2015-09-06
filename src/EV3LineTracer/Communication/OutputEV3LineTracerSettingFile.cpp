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
	output.writeToken("ConstractTime");

	std::string construct_time = ev3LineTracer.getConstructTimeString();
	output.writeToken(construct_time);
	output.newLine();

	WriteDiscountRate(ev3LineTracer).process(output);

	WriteEV3LineTracerSetting(ev3LineTracer).process(output);
}

} /* namespace RL */
