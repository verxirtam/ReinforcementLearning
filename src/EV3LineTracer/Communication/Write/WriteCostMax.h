/*
 * WriteCostMax.h
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#ifndef WRITECOSTMAX_H_
#define WRITECOSTMAX_H_

#include "../../../Communication/Write/WriteValue.h"
#include "../../../RLUtility.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class WriteCostMax: public WriteValue<EV3LineTracer, real>
{
public:
	WriteCostMax(const EV3LineTracer& ev3):WriteValue(ev3, &EV3LineTracer::getCostMax)
	{
	}
	virtual ~WriteCostMax()
	{
	}
};


} /* namespace RL */

#endif /* WRITECOSTMAX_H_ */
