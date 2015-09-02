/*
 * OutputCommandSetMDP.h
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#ifndef OUTPUTCOMMANDSETMDP_H_
#define OUTPUTCOMMANDSETMDP_H_

#include "../../Communication/OutputProcedure.h"
#include "../../Communication/OutputContext.h"
#include "../../Communication/Write/WriteRegularPolicy.h"
#include "../EV3LineTracer.h"
#include "Write/WriteControl.h"
#include "Write/WriteInterval.h"
#include "Write/WriteCostMax.h"
#include "Write/WriteState.h"
#include "Write/WriteStateCount.h"

namespace RL
{

class OutputCommandSetMDP: public OutputProcedure
{
private:
	const EV3LineTracer& ev3LineTracer;
public:
	OutputCommandSetMDP(const EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~OutputCommandSetMDP()
	{
	}
	virtual void process(OutputContext& output);

};

} /* namespace RL */

#endif /* OUTPUTCOMMANDSETMDP_H_ */
