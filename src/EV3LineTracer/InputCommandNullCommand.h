/*
 * InputCommandNullCommand.h
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#ifndef INPUTCOMMANDNULLCOMMAND_H_
#define INPUTCOMMANDNULLCOMMAND_H_

#include "../Communication/InputProcedure.h"

namespace RL
{

class InputCommandNullCommand: public InputProcedure
{
public:
	InputCommandNullCommand()
	{
	}
	virtual ~InputCommandNullCommand()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* INPUTCOMMANDNULLCOMMAND_H_ */
