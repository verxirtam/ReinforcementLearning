/*
 * OutputEV3LineTracerDestructFile.cpp
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#include "OutputEV3LineTracerDestructFile.h"


namespace RL
{
void OutputEV3LineTracerDestructFile::process(OutputContext& output)
{
	output.writeToken(TimeToString::toString(destructTime));
	output.newLine();
}

} /* namespace RL */
