/*
 * ReadInterval.h
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#ifndef READINTERVAL_H_
#define READINTERVAL_H_

#include "../../../Communication/Read/ReadValue.h"
#include "../../../RLUtility.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class ReadInterval: public ReadValue<EV3LineTracer, idx>
{
public:
	ReadInterval(EV3LineTracer &ev3):ReadValue(ev3,&EV3LineTracer::setInterval)
	{
	}
	virtual ~ReadInterval()
	{
	}
};

} /* namespace RL */

#endif /* READINTERVAL_H_ */
