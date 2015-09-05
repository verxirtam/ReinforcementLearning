/*
 * EV3LineTracerSetting.cpp
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#include "WriteEV3LineTracerSetting.h"


namespace RL
{
void WriteEV3LineTracerSetting::process(OutputContext& output)
{
	WriteInterval(ev3LineTracer).process(output);

	WriteCostMax(ev3LineTracer).process(output);

	WriteStateCount(ev3LineTracer).process(output);

	WriteState(ev3LineTracer).process(output);

	WriteControl(ev3LineTracer).process(output);

	WriteRegularPolicy<EV3LineTracer>(ev3LineTracer).process(output);
}

} /* namespace RL */
