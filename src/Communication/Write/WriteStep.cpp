/*
 * WriteStep.cpp
 *
 *  Created on: 2015/09/05
 *      Author: daisuke
 */

#include "WriteStep.h"


namespace RL
{
void WriteStep::process(OutputContext& output)
{
	output.writeToken(std::to_string(stepIndex));
	output.writeToken(std::to_string(step.state));
	output.writeToken(std::to_string(step.control));
	output.writeToken(std::to_string(step.cost));
	output.newLine();
}
} /* namespace RL */
