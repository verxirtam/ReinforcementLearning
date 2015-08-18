/*
 * WriteCostMax.h
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#ifndef WRITECOSTMAX_H_
#define WRITECOSTMAX_H_

#include <string>
#include "../../../Communication/OutputProcedure.h"
#include "../../../Communication/OutputContext.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class WriteCostMax: public OutputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	WriteCostMax(EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~WriteCostMax()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* WRITECOSTMAX_H_ */
