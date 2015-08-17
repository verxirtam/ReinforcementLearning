/*
 * OutputMessage.cpp
 *
 *  Created on: 2015/08/17
 *      Author: daisuke
 */

#include "OutputMessage_1_0.h"

namespace RL
{
void OutputMessage_1_0::process(OutputContext& output)
{
	output.writeToken("MESSAGE_1.0");
	output.newLine();

	this->messageBody.process(output);

	//空行
	output.newLine();

	output.flush();
}

} /* namespace RL */
