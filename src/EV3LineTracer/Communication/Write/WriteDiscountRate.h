/*
 * WriteDiscountRate.h
 *
 *  Created on: 2015/09/07
 *      Author: daisuke
 */

#ifndef WRITEDISCOUNTRATE_H_
#define WRITEDISCOUNTRATE_H_

#include "../../../Communication/Write/WriteValue.h"
#include "../../../RLUtility.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class WriteDiscountRate: public WriteValue<EV3LineTracer,real>
{
public:
	WriteDiscountRate(const EV3LineTracer& ev3):WriteValue(ev3, &EV3LineTracer::getDiscountRate)
	{
	}
	virtual ~WriteDiscountRate()
	{
	}
};

} /* namespace RL */

#endif /* WRITEDISCOUNTRATE_H_ */
