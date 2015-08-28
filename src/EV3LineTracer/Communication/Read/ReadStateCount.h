/*
 * ReadStateCount.h
 *
 *  Created on: 2015/01/12
 *      Author: daisuke
 */

#ifndef READSTATECOUNT_H_
#define READSTATECOUNT_H_

#include "../../../Communication/Read/ReadValue.h"
#include "../../../RLUtility.h"
#include "../../EV3LineTracer.h"

namespace RL
{


class ReadStateCount: public ReadValue<EV3LineTracer, idx>
{
public:
	ReadStateCount(EV3LineTracer& ev3):ReadValue(ev3, &EV3LineTracer::setStateCount)
	{
	}
	virtual ~ReadStateCount()
	{
	}
};

} /* namespace RL */

#endif /* READSTATECOUNT_H_ */
