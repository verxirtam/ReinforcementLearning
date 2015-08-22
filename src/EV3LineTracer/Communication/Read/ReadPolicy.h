/*
 * ReadPolicy.h
 *
 *  Created on: 2015/08/23
 *      Author: daisuke
 */

#ifndef READPOLICY_H_
#define READPOLICY_H_

#include "../../../Communication/InputProcedure.h"
#include "../../../Policy.h"
#include "../../../RLUtility.h"
#include "ReadSinglePolicy.h"

namespace RL
{

class ReadPolicy: public InputProcedure
{
private:
	Policy& policy;
public:
	ReadPolicy(Policy& p):policy(p)
	{
	}
	virtual ~ReadPolicy()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READPOLICY_H_ */
