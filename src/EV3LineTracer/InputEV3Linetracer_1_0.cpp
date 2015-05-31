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
	//バージョン文字列のチェック
	input.skipToken(VERSION_STRING);
	input.skipReturn();

	//Intervalの読み取り
	ReadInterval ri(ev3LineTracer);
	ri.process(input);

	//State数の読み取り
	ReadStateCount rsc(ev3LineTracer);
	ri.process(input);

	//Stateの読み取り
	ReadState rs(ev3LineTracer);
	rs.process(input);


}
} /* namespace RL */
