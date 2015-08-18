/*
 * InputCommandNullCommand.cpp
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#include "InputCommandNullCommand.h"

namespace RL
{

void InputCommandNullCommand::process(InputContext &input)
{
	input.skipToken("NullCommand");
	input.skipReturn();
	input.skipToken("OK");
	input.skipReturn();
}

} /* namespace RL */
