/*
 * WriteStep.h
 *
 *  Created on: 2015/09/05
 *      Author: daisuke
 */

#ifndef WRITESTEP_H_
#define WRITESTEP_H_

#include <string>

#include "../../Episode.h"
#include "../../RLUtility.h"
#include "../OutputProcedure.h"
#include "../OutputContext.h"

namespace RL
{

class WriteStep: public OutputProcedure
{
	const RL::Step& step;
	idx stepIndex;
public:
	WriteStep(const RL::Step& step_, idx stepIndex_):step(step_),stepIndex(stepIndex_)
	{
	}
	virtual ~WriteStep()
	{
	}
	void setStepIndex(idx n)
	{
		stepIndex = n;
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* WRITESTEP_H_ */
