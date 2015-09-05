/*
 * OutputEV3LineTracerConstructFile.cpp
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#include "OutputEV3LineTracerConstructFile.h"

namespace RL
{
void OutputEV3LineTracerConstructFile::process(OutputContext& output)
{
	output.writeToken(ev3LineTracer.getConstructTimeString());
	output.newLine();
}
} /* namespace RL */
