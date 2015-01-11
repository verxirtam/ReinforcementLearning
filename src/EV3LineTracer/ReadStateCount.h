/*
 * ReadStateCount.h
 *
 *  Created on: 2015/01/12
 *      Author: daisuke
 */

#ifndef READSTATECOUNT_H_
#define READSTATECOUNT_H_

#include "../RLUtility.h"
#include "../StringToData.h"
#include "EV3LineTracer.h"
#include "../Communication/InputProcedure.h"

namespace RL
{


class ReadStateCount: public InputProcedure
{
private:
	EV3LineTracer &ev3LineTracer;
public:
	ReadStateCount(EV3LineTracer &ev3);
	virtual ~ReadStateCount();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READSTATECOUNT_H_ */
