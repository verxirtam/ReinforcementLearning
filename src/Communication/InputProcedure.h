/*
 * InputProcedure.h
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#ifndef INPUTPROCEDURE_H_
#define INPUTPROCEDURE_H_

#include "InputContext.h"


namespace RL
{

class InputProcedure
{
public:
	virtual ~InputProcedure(){};
	virtual void process(InputContext &input)=0;
};

} /* namespace RL */

#endif /* INPUTPROCEDURE_H_ */
