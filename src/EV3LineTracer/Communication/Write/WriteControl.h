/*
 * WriteControl.h
 *
 *  Created on: 2015/08/15
 *      Author: daisuke
 */

#ifndef WRITECONTROL_H_
#define WRITECONTROL_H_

#include "../../../RLUtility.h"
#include "../../../Communication/OutputProcedure.h"
#include "../../EV3LineTracer.h"
#include "WriteSingleControl.h"

namespace RL
{

class WriteControl: public OutputProcedure
{
private:
	const EV3LineTracer& ev3LineTracer;
public:
	WriteControl(const EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~WriteControl()
	{
	}
	virtual void process(OutputContext& output);
};
} /* namespace RL */

#endif /* WRITECONTROL_H_ */
