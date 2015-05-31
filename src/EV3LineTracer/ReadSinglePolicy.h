/*
 * ReadSinglePolicy.h
 *
 *  Created on: 2015/05/31
 *      Author: daisuke
 */

#ifndef READSINGLEPOLICY_H_
#define READSINGLEPOLICY_H_

#include "../Communication/InputProcedure.h"
#include "../Communication/InputContext.h"
#include "../Policy.h"
#include "../RLUtility.h"
#include "../StringToData.h"
#include "EV3LineTracer.h"

namespace RL
{

class ReadSinglePolicy: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
	Policy& policy;
	idx stateIndex;
public:
	ReadSinglePolicy(EV3LineTracer& ev3, Policy& p, idx i=0);
	void setPolicy(Policy& p)
	{
		policy = p;
	}
	void setStateIndex(idx i)
	{
		stateIndex = i;
	}
	virtual ~ReadSinglePolicy();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READSINGLEPOLICY_H_ */
