/*
 * WriteSingleControl.h
 *
 *  Created on: 2015/08/12
 *      Author: daisuke
 */

#ifndef WRITESINGLECONTROL_H_
#define WRITESINGLECONTROL_H_

#include "../../../RLUtility.h"
#include "../../../Communication/OutputProcedure.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class WriteSingleControl: public OutputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
	idx stateIndex;
	idx controlIndex;
public:
	WriteSingleControl(EV3LineTracer& ev3,idx i,idx u):
		ev3LineTracer(ev3),stateIndex(i),controlIndex(u)
	{

	}
	virtual void process(OutputContext& output);
	virtual ~WriteSingleControl()
	{
	}
	void setStateIndex(idx i)
	{
		stateIndex = i;
	}
	void setControlIndex(idx u)
	{
		controlIndex = u;
	}
	void setInddex(idx i,idx u)
	{
		setStateIndex(i);
		setControlIndex(u);
	}
};

} /* namespace RL */

#endif /* WRITESINGLECONTROL_H_ */
