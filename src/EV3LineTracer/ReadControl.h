/*
 * ReadControl.h
 *
 *  Created on: 2015/01/25
 *      Author: daisuke
 */

#ifndef READCONTROL_H_
#define READCONTROL_H_
#include "../Communication/InputContext.h"
#include "../Communication/InputProcedure.h"
#include "../StringToData.h"
#include "EV3LineTracer.h"

namespace RL
{

class ReadControl: public InputProcedure
{
private:
	EV3LineTracer &ev3LineTracer;
public:
	ReadControl(EV3LineTracer &ev3);
	virtual ~ReadControl();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READCONTROL_H_ */
