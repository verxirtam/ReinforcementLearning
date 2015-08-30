/*
 * InputCommandSetCurrentPolicy.h
 *
 *  Created on: 2015/08/30
 *      Author: daisuke
 */

#ifndef INPUTCOMMANDSETCURRENTPOLICY_H_
#define INPUTCOMMANDSETCURRENTPOLICY_H_


#include <iostream>
#include <sstream>
#include <string>

#include "../../Communication/InputProcedure.h"
#include "../../Communication/InputContext.h"
#include "../../Communication/Read/ReadStochasticPolicy.h"
#include "../../Policy.h"
#include "../EV3LineTracer.h"

namespace RL
{

class InputCommandSetCurrentPolicy: public InputProcedure
{
private:
	const EV3LineTracer& ev3LineTracer;
	void processOnNG(InputContext& input);
public:
	InputCommandSetCurrentPolicy(const EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~InputCommandSetCurrentPolicy()
	{
	}
	virtual void process(InputContext &input);

};

} /* namespace RL */

#endif /* INPUTCOMMANDSETCURRENTPOLICY_H_ */
