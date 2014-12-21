/*
 * InputEV3Linetracer_1_0.cpp
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#include "InputEV3Linetracer_1_0.h"

namespace RL
{
const std::string InputEV3Linetracer_1_0::VERSION_STRING = "EV3LineTracer_1.0";

InputEV3Linetracer_1_0::InputEV3Linetracer_1_0(EV3LineTracer &ev3) :
		ev3LineTracer(ev3)
{
}

InputEV3Linetracer_1_0::~InputEV3Linetracer_1_0()
{
}
void InputEV3Linetracer_1_0::process(InputContext &input)
{
	input.skipToken(VERSION_STRING);
	input.skipReturn();

	ReadInterval ri(ev3LineTracer);
	ri.process(input);
}
} /* namespace RL */
