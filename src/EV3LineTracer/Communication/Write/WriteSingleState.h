/*
 * WriteSingleState.h
 *
 *  Created on: 2015/08/11
 *      Author: daisuke
 */

#ifndef WRITESINGLESTATE_H_
#define WRITESINGLESTATE_H_

#include "../../../Communication/OutputProcedure.h"
#include "../../../RLUtility.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class WriteSingleState: public OutputProcedure
{
private:
	const EV3LineTracer& ev3LineTracer;
	idx index;
public:
	WriteSingleState(const EV3LineTracer& ev3,idx i):ev3LineTracer(ev3),index(i)
	{

	}
	void setStateIndex(idx i)
	{
		index=i;
	}
	virtual ~WriteSingleState(){}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* WRITESINGLESTATE_H_ */
