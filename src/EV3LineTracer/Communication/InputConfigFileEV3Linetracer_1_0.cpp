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
	ReadInterval ri(ev3LineTracer);
	ri.process(input);

	//CostMaxの読み取り
	ReadCostMax rcm(ev3LineTracer);
	rcm.process(input);

	//State数の読み取り
	ReadStateCount rsc(ev3LineTracer);
	rsc.process(input);

	//Stateの読み取り
	ReadState rs(ev3LineTracer);
	rs.process(input);

	//Controlの読み取り
	ReadControl rc(ev3LineTracer);
	rc.process(input);

	//RegularPolicyの読み取り
	ReadRegularPolicy<EV3LineTracer> rrp(ev3LineTracer);
	rrp.process(input);

	//CurrentPolicyをRegularPolicyとして設定する
	ev3LineTracer.setCurrentPolicy(ev3LineTracer.getRegularPolicy());
}
} /* namespace RL */
