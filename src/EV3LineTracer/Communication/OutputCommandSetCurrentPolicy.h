/*
 * OutputCommandSetCurrentPolicy.h
 *
 *  Created on: 2015/08/30
 *      Author: daisuke
 */

#ifndef OUTPUTCOMMANDSETCURRENTPOLICY_H_
#define OUTPUTCOMMANDSETCURRENTPOLICY_H_

#include "../../Communication/OutputProcedure.h"
#include "../../Communication/Write/WriteCurrentPolicy.h"
#include "../EV3LineTracer.h"

namespace RL
{

class OutputCommandSetCurrentPolicy: public OutputProcedure
{
private:
	const EV3LineTracer& ev3LineTracer;
public:
	OutputCommandSetCurrentPolicy(const EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~OutputCommandSetCurrentPolicy()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTCOMMANDCURRENTPOLICY_H_ */
