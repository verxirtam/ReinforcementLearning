/*
 * OutputCommandNullCommand.cpp
 *
 *  Created on: 2015/08/17
 *      Author: daisuke
 */

#include "OutputCommandNullCommand.h"

namespace RL
{
void OutputCommandNullCommand::process(OutputContext& output)
{
	output.writeToken("NullCommand");
	output.newLine();
}

} /* namespace RL */
