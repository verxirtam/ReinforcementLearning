/*
 * InputEV3LineTracer_1_0.h
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#ifndef INPUTEV3LINETRACER_1_0_H_
#define INPUTEV3LINETRACER_1_0_H_

#include "../Communication/InputProcedure.h"

namespace RL
{

class InputEV3LineTracer_1_0: public InputProcedure
{
private:
	InputProcedure& inputProcedure;
public:
	InputEV3LineTracer_1_0(InputProcedure& ip):inputProcedure(ip)
	{
	}
	virtual ~InputEV3LineTracer_1_0()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* INPUTEV3LINETRACER10_H_ */
