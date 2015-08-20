/*
 * WriteRegularPolicy.h
 *
 *  Created on: 2015/08/16
 *      Author: daisuke
 */

#ifndef WRITEREGULARPOLICY_H_
#define WRITEREGULARPOLICY_H_

#include "../../../Communication/OutputProcedure.h"
#include "../../../Policy.h"
#include "../../../RLUtility.h"
#include "../../EV3LineTracer.h"
#include "WriteSinglePolicy.h"

namespace RL
{

class WritePolicy: public OutputProcedure
{
private:
	Policy& policy;
public:
	WritePolicy(Policy& p):policy(p)
	{

	}
	virtual ~WritePolicy()
	{

	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* WRITEREGULARPOLICY_H_ */