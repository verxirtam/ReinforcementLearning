/*
 * ReadEV3LineTracerSetting.cpp
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#include "ReadEV3LineTracerSetting.h"


namespace RL
{
void ReadEV3LineTracerSetting::process(InputContext &input)
{
	//Intervalの読み取り
	ReadInterval ri(ev3LineTracer);
	ri.process(input);

	//DiscountRateの読み取り
	ReadDiscountRate rd(ev3LineTracer);
	rd.process(input);

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
	ev3LineTracer.setCurrentPolicyLocal(ev3LineTracer.getRegularPolicy());
}

} /* namespace RL */
