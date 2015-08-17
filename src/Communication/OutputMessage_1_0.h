/*
 * OutputMessage.h
 *
 *  Created on: 2015/08/17
 *      Author: daisuke
 */

#ifndef OUTPUTMESSAGE_1_0_H_
#define OUTPUTMESSAGE_1_0_H_

#include "OutputProcedure.h"

namespace RL
{

class OutputMessage_1_0: public OutputProcedure
{
private:
	OutputProcedure& messageBody;
public:
	OutputMessage_1_0(OutputProcedure& output):messageBody(output)
	{
	}
	virtual ~OutputMessage_1_0()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTMESSAGE_H_ */
