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
#include "InputEV3Linetracer_1_0.h"


namespace RL
{

class InputConfigFile: public InputProcedure
{
private:
	InputProcedure &body;
public:
	InputConfigFile(InputProcedure &b);
	virtual ~InputConfigFile();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* INPUTCONFIGFILE_H_ */
