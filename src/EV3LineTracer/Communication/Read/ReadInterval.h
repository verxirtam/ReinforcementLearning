/*
 * ReadInterval.h
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#ifndef READINTERVAL_H_
#define READINTERVAL_H_

#include "../../../RLUtility.h"
#include "../../../StringToData.h"
#include "../../EV3LineTracer.h"
#include "../../../Communication/InputProcedure.h"

namespace RL
{

class ReadInterval: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	ReadInterval(EV3LineTracer &ev3);
	virtual ~ReadInterval();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READINTERVAL_H_ */
