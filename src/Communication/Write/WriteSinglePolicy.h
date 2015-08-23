/*
 * WriteSinglePolicy.h
 *
 *  Created on: 2015/08/16
 *      Author: daisuke
 */

#ifndef WRITESINGLEPOLICY_H_
#define WRITESINGLEPOLICY_H_

#include <string>

#include "../OutputProcedure.h"
#include "../../Policy.h"
#include "../../RLUtility.h"
#include "../OutputContext.h"

namespace RL
{

class WriteSinglePolicy: public OutputProcedure
{
private:
	Policy& policy;
	idx stateIndex;
public:
	WriteSinglePolicy(Policy& p,idx i):policy(p),stateIndex(i)
	{
	}
	virtual ~WriteSinglePolicy()
	{
	}
	virtual void process(OutputContext& output);
	void setStateIndex(idx i)
	{
		stateIndex = i;
	}
};

} /* namespace RL */

#endif /* WRITESINGLEPOLICY_H_ */
