/*
 * WriteInterval.h
 *
 *  Created on: 2015/08/11
 *      Author: daisuke
 */

#ifndef WRITEINTERVAL_H_
#define WRITEINTERVAL_H_

#include "../../../Communication/OutputProcedure.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class WriteInterval: public OutputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	WriteInterval(EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~WriteInterval(){}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* WRITEINTERVAL_H_ */
