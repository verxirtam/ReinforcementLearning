/*
 * OutputMessage.h
 *
 *  Created on: 2015/08/17
 *      Author: daisuke
 */

#ifndef OUTPUTMESSAGE_H_
#define OUTPUTMESSAGE_H_

#include "OutputProcedure.h"

namespace RL
{

class OutputMessage: public OutputProcedure
{
private:
	OutputProcedure& messageBody;
public:
	OutputMessage(OutputProcedure& output):messageBody(output)
	{
	}
	virtual ~OutputMessage()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTMESSAGE_H_ */
