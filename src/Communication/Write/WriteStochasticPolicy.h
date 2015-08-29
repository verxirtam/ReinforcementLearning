/*
 * WriteStochasticPolicy.h
 *
 *  Created on: 2015/08/25
 *      Author: daisuke
 */

#ifndef WRITESTOCHASTICPOLICY_H_
#define WRITESTOCHASTICPOLICY_H_


#include <string>

#include "../../Policy.h"
#include "../../Random.h"
#include "../../RLUtility.h"
#include "../OutputProcedure.h"
#include "../OutputContext.h"


namespace RL
{

class WriteStochasticPolicy: public OutputProcedure
{
private:
	const StochasticPolicy& stochasticPolicy;
public:
	WriteStochasticPolicy(const StochasticPolicy& sp):stochasticPolicy(sp)
	{
	}
	virtual ~WriteStochasticPolicy()
	{
	}
	virtual void process(OutputContext& output);

};

} /* namespace RL */

#endif /* WRITESTOCHASTICPOLICY_H_ */
