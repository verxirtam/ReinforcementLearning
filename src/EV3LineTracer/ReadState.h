/*
 * ReadState.h
 *
 *  Created on: 2015/01/24
 *      Author: daisuke
 */

#ifndef READSTATE_H_
#define READSTATE_H_

#include "../RLUtility.h"
#include "../Communication/InputContext.h"
#include "../Communication/InputProcedure.h"
#include "../StringToData.h"
#include "EV3LineTracer.h"

namespace RL
{

class ReadState: public InputProcedure
{
private:
	EV3LineTracer ev3LineTracer;
public:
	ReadState(EV3LineTracer &ev3);
	virtual ~ReadState();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READSTATE_H_ */
