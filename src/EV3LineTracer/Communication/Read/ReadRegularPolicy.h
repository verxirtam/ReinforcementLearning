/*
 * ReadRegularPolicy.h
 *
 *  Created on: 2015/05/31
 *      Author: daisuke
 */

#ifndef READREGULARPOLICY_H_
#define READREGULARPOLICY_H_

#include "../../../Communication/InputProcedure.h"
#include "../../EV3LineTracer.h"
#include "../../../Communication/Read/ReadPolicy.h"




namespace RL
{

class ReadRegularPolicy: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	ReadRegularPolicy(EV3LineTracer& ev3) : ev3LineTracer(ev3)
	{
	}
	virtual ~ReadRegularPolicy()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READREGULARPOLICY_H_ */
