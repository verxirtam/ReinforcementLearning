/*
 * WriteState.h
 *
 *  Created on: 2015/08/12
 *      Author: daisuke
 */

#ifndef WRITESTATE_H_
#define WRITESTATE_H_

#include "../../../RLUtility.h"
#include "../../../Communication/OutputProcedure.h"
#include "../../EV3LineTracer.h"
#include "WriteSingleState.h"

namespace RL
{

class WriteState: public OutputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	WriteState(EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~WriteState()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* WRITESTATE_H_ */
