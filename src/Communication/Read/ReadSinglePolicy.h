/*
 * ReadSinglePolicy.h
 *
 *  Created on: 2015/05/31
 *      Author: daisuke
 */

#ifndef READSINGLEPOLICY_H_
#define READSINGLEPOLICY_H_

#include "../InputProcedure.h"
#include "../InputContext.h"
#include "../../Policy.h"
#include "../../RLUtility.h"
#include "../../StringToData.h"

namespace RL
{

class ReadSinglePolicy: public InputProcedure
{
private:
	Policy& policy;
	idx stateIndex;
public:
	ReadSinglePolicy(Policy& p, idx i = 0 ):
			policy(p),
			stateIndex(0)
	{
		setStateIndex(i);

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
