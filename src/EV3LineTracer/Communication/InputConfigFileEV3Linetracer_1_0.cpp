/*
 * InputConfigFileEV3Linetracer_1_0.cpp
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#include "InputConfigFileEV3Linetracer_1_0.h"



namespace RL
{
const std::string InputConfigFileEV3Linetracer_1_0::VERSION_STRING = "EV3LineTracer_1.0";

InputConfigFileEV3Linetracer_1_0::InputConfigFileEV3Linetracer_1_0(EV3LineTracer &ev3) :
		ev3LineTracer(ev3)
{
}

InputConfigFileEV3Linetracer_1_0::~InputConfigFileEV3Linetracer_1_0()
{
}
void InputConfigFileEV3Linetracer_1_0::process(InputContext &input)
{
	//Intervalの読み取り
	ReadEV3LineTracerSetting r(ev3LineTracer);
	r.process(input);
}
} /* namespace RL */
