/*
 * OutputNullCommand.cpp
 *
 *  Created on: 2015/08/17
 *      Author: daisuke
 */

#include "OutputNullCommand.h"

namespace RL
{
void OutputNullCommand::process(OutputContext& output)
{
	output.writeToken("NullCommand");
	output.newLine();
}

} /* namespace RL */
