/*
 * WriteInterval.h
 *
 *  Created on: 2015/08/11
 *      Author: daisuke
 */

#ifndef WRITEINTERVAL_H_
#define WRITEINTERVAL_H_

#include "../../../Communication/Write/WriteValue.h"
#include "../../../RLUtility.h"
#include "../../EV3LineTracer.h"

namespace RL
{
class WriteInterval: public WriteValue<EV3LineTracer, idx>
{
public:
	WriteInterval(EV3LineTracer& ev3):WriteValue(ev3,&EV3LineTracer::getInterval)
	{
	}
	virtual ~WriteInterval()
	{
	}
};

} /* namespace RL */

#endif /* WRITEINTERVAL_H_ */
