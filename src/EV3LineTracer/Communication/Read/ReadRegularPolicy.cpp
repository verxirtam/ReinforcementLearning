/*
 * ReadRegularPolicy.cpp
 *
 *  Created on: 2015/05/31
 *      Author: daisuke
 */

#include "ReadRegularPolicy.h"




namespace RL
{

void ReadRegularPolicy::process(InputContext &input)
{
	//読み取り用ポリシーの設定
	idx statecount = ev3LineTracer.getStateCount();
	Policy p(statecount);

	ReadPolicy rp(p);

	//ポリシーの読み取り
	rp.process(input);

	//pがev3LineTracerと適合しているかチェックする
	p.verify(ev3LineTracer);
	//ev3LineTracer.RegularPolicyへの書き込み
	ev3LineTracer.setRegularPolicy(p);

}
} /* namespace RL */
