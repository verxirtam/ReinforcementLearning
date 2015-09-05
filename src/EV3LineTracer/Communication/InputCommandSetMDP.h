/*
 * InputCommandSetMDP.h
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#ifndef INPUTCOMMANDSETMDP_H_
#define INPUTCOMMANDSETMDP_H_

#include <sstream>
#include <string>
#include <cmath>
#include <iostream>



#include "../../Communication/InputProcedure.h"
#include "../../Communication/InputContext.h"
#include "../../Policy.h"
#include "../../RLUtility.h"
#include "../EV3LineTracer.h"
#include "Read/ReadEV3LineTracerSetting.h"

namespace RL
{

class InputCommandSetMDP: public InputProcedure
{
private:
	const EV3LineTracer& ev3LineTracer;

	bool equalsOfEV3LineTracer(const EV3LineTracer& ev3)const;
	void processOnNG(InputContext& input);

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
