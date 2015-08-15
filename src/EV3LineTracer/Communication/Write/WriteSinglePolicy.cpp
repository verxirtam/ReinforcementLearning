/*
 * WriteSinglePolicy.cpp
 *
 *  Created on: 2015/08/16
 *      Author: daisuke
 */

#include "WriteSinglePolicy.h"



namespace RL
{

void WriteSinglePolicy::process(OutputContext& output)
{
	output.writeToken(std::to_string(stateIndex));
	output.writeToken(std::to_string(this->policy[stateIndex]));
	output.newLine();
}

} /* namespace RL */
