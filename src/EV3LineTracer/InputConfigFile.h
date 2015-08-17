/*
 * InputConfigFile.h
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#ifndef INPUTCONFIGFILE_H_
#define INPUTCONFIGFILE_H_

#include "../Communication/InputContext.h"
#include "../Communication/InputProcedure.h"
#include "InputConfigFileEV3Linetracer_1_0.h"


namespace RL
{

class InputConfigFile: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	InputConfigFile(EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~InputConfigFile()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* INPUTCONFIGFILE_H_ */
