/*
 * InputCommandSetMDP.h
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#ifndef INPUTCOMMANDSETMDP_H_
#define INPUTCOMMANDSETMDP_H_

#include "../../Communication/InputProcedure.h"
#include "../EV3LineTracer.h"

namespace RL
{

class InputCommandSetMDP: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	InputCommandSetMDP(EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~InputCommandSetMDP()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* INPUTCOMMANDSETMDP_H_ */
