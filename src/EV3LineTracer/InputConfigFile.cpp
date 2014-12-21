/*
 * InputConfigFile.cpp
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#include "InputConfigFile.h"

namespace RL
{

InputConfigFile::InputConfigFile(InputProcedure &b):body(b)
{
}

InputConfigFile::~InputConfigFile()
{
}
void InputConfigFile::process(InputContext &input)
{
	body.process(input);
}

} /* namespace RL */
