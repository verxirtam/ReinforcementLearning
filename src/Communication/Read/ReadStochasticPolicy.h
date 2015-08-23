/*
 * ReadStochasticPolicy.h
 *
 *  Created on: 2015/08/23
 *      Author: daisuke
 */

#ifndef READSTOCHASTICPOLICY_H_
#define READSTOCHASTICPOLICY_H_

#include <string>
#include <vector>

#include "../../Policy.h"
#include "../../Random.h"
#include "../../RLUtility.h"
#include "../../StringToData.h"
#include "../InputProcedure.h"
#include "../InputContext.h"


namespace RL
{

class ReadStochasticPolicy: public InputProcedure
{
private:
	StochasticPolicy& stochasticPolicy;
public:
	ReadStochasticPolicy(StochasticPolicy& sp):stochasticPolicy(sp)
	{
	}
	virtual ~ReadStochasticPolicy()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READSTOCHASTICPOLICY_H_ */
