/*
 * ReadCostMax.h
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#ifndef READCOSTMAX_H_
#define READCOSTMAX_H_

#include "../../../Communication/Read/ReadValue.h"
#include "../../../RLUtility.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class ReadCostMax: public ReadValue<EV3LineTracer, real>
{
public:
	ReadCostMax(EV3LineTracer& ev3):ReadValue(ev3, &EV3LineTracer::setCostMax)
	{
	}
	virtual ~ReadCostMax()
	{
	}
};

} /* namespace RL */

#endif /* READCOSTMAX_H_ */
