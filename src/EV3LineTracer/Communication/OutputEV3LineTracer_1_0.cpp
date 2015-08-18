/*
 * OutputEV3LineTracer10.cpp
 *
 *  Created on: 2015/08/17
 *      Author: daisuke
 */

#include "OutputEV3LineTracer_1_0.h"

namespace RL
{
void OutputEV3LineTracer_1_0::process(OutputContext& output)
{
	//EV3のヴァージョンの書き込み
	output.writeToken("EV3LineTracer_1.0");
	output.newLine();

	//後続の処理の実行
	this->outputProcedure.process(output);
}
} /* namespace RL */
