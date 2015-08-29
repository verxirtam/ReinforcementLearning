/*
 * WriteStateCount.h
 *
 *  Created on: 2015/08/11
 *      Author: daisuke
 */

#ifndef WRITESTATECOUNT_H_
#define WRITESTATECOUNT_H_

#include "../../../Communication/Write/WriteValue.h"
#include "../../../RLUtility.h"
#include "../../EV3LineTracer.h"

namespace RL
{
class WriteStateCount: public WriteValue<EV3LineTracer, idx>
{
public:
	WriteStateCount(EV3LineTracer& ev3):WriteValue(ev3, &EV3LineTracer::getStateCount)
	{
	}
	virtual ~WriteStateCount(){}
};
}
#endif /* WRITESTATECOUNT_H_ */
