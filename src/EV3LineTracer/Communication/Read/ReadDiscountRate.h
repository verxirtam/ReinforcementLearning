/*
 * ReadDiscountRate.h
 *
 *  Created on: 2015/09/07
 *      Author: daisuke
 */

#ifndef READDISCOUNTRATE_H_
#define READDISCOUNTRATE_H_

#include "../../../Communication/Read/ReadValue.h"
#include "../../../RLUtility.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class ReadDiscountRate: public ReadValue<EV3LineTracer,real>
{
public:
	ReadDiscountRate(EV3LineTracer& ev3):ReadValue(ev3, &EV3LineTracer::setDiscountRate)
	{
	}
	virtual ~ReadDiscountRate()
	{
	}
};

} /* namespace RL */

#endif /* READDISCOUNTRATE_H_ */
