/*
 * WriteRegularPolicy.h
 *
 *  Created on: 2015/08/21
 *      Author: daisuke
 */

#ifndef WRITEREGULARPOLICY_H_
#define WRITEREGULARPOLICY_H_

#include "../../../Communication/OutputProcedure.h"
#include "../../../Policy.h"
#include "../../EV3LineTracer.h"
#include "WritePolicy.h"

namespace RL
{

class WriteRegularPolicy: public OutputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	WriteRegularPolicy(EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~WriteRegularPolicy()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* WRITEREGULARPOLICY_H_ */