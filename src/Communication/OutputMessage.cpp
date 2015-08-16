/*
 * OutputMessage.cpp
 *
 *  Created on: 2015/08/17
 *      Author: daisuke
 */

#include "OutputMessage.h"

namespace RL
{
void OutputMessage::process(OutputContext& output)
{
	output.writeToken("MESSAGE_1.0");
	output.newLine();

	this->outputProcedure.process(output);
}

} /* namespace RL */
