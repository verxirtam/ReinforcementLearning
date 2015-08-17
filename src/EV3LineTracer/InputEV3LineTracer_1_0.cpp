/*
 * InputEV3LineTracer_1_0.cpp
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#include "InputEV3LineTracer_1_0.h"

namespace RL
{
void InputEV3LineTracer_1_0::process(InputContext &input)
{
	input.skipToken("EV3LineTracer_1.0");
	input.skipReturn();

	inputProcedure.process(input);
}

} /* namespace RL */
