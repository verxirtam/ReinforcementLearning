/*
 * InputMessage_1_0.h
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#ifndef INPUTMESSAGE_1_0_H_
#define INPUTMESSAGE_1_0_H_

#include "InputProcedure.h"

namespace RL
{

class InputMessage_1_0: public InputProcedure
{
private:
	InputProcedure& inputProcedure;
public:
	InputMessage_1_0(InputProcedure& ip):inputProcedure(ip)
	{
	}
	virtual ~InputMessage_1_0()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* INPUTMESSAGE_1_0_H_ */
