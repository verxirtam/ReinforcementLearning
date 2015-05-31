/*
 * InputEV3Linetracer_1_0.h
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#ifndef INPUTEV3LINETRACER_1_0_H_
#define INPUTEV3LINETRACER_1_0_H_


#include "../Communication/InputContext.h"
#include "../Communication/InputProcedure.h"
#include <string>
#include "EV3LineTracer.h"
#include "ReadInterval.h"
#include "ReadStateCount.h"
#include "ReadSingleState.h"


namespace RL
{

class InputEV3Linetracer_1_0: public InputProcedure
{
private:
	EV3LineTracer &ev3LineTracer;
public:
	static const std::string VERSION_STRING;
	InputEV3Linetracer_1_0(EV3LineTracer &ev3);
	virtual ~InputEV3Linetracer_1_0();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* INPUTEV3LINETRACER_1_0_H_ */
