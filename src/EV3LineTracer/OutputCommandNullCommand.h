/*
 * OutputCommandNullCommand.h
 *
 *  Created on: 2015/08/17
 *      Author: daisuke
 */

#ifndef OUTPUTCOMMANDNULLCOMMAND_H_
#define OUTPUTCOMMANDNULLCOMMAND_H_

#include "../Communication/OutputProcedure.h"

namespace RL
{

class OutputNullCommand: public OutputProcedure
{
public:
	OutputNullCommand()
	{
	}
	virtual ~OutputNullCommand()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTNULLCOMMAND_H_ */
