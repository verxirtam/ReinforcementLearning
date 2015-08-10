/*
 * WriteStateCount.h
 *
 *  Created on: 2015/08/11
 *      Author: daisuke
 */

#ifndef WRITESTATECOUNT_H_
#define WRITESTATECOUNT_H_

#include "../../../Communication/OutputProcedure.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class WriteStateCount: public OutputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	WriteStateCount(EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~WriteStateCount(){}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* WRITESTATECOUNT_H_ */
