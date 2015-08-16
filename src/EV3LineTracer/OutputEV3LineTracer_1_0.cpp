/*
 * OutputEV3LineTracer10.cpp
 *
 *  Created on: 2015/08/17
 *      Author: daisuke
 */

#include "OutputEV3LineTracer_1_0.h"

namespace RL
{
void OutputEV3LineTracer_1_0::process(OutputContext& o)
{
	//EV3のヴァージョンの書き込み
	o.writeToken("EV3LineTracer_1.0");
	o.newLine();

	//後続の処理の実行
	this->output.process(o);
}
} /* namespace RL */
