/*
 * OutputProcedure.h
 *
 *  Created on: 2015/08/11
 *      Author: daisuke
 */

#ifndef OUTPUTPROCEDURE_H_
#define OUTPUTPROCEDURE_H_

#include "OutputContext.h"

namespace RL
{

class OutputProcedure
{
public:
	virtual ~OutputProcedure(){};
	virtual void process(OutputContext& output)=0;
};

} /* namespace RL */

#endif /* OUTPUTPROCEDURE_H_ */
