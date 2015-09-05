/*
 * ReadStep.h
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#ifndef READSTEP_H_
#define READSTEP_H_

#include <iostream>

#include "../../StringToData.h"
#include "../../Episode.h"
#include "../../RLUtility.h"
#include "../InputProcedure.h"
#include "../InputContext.h"


namespace RL
{

class ReadStep: public InputProcedure
{
private:
	Step& step;
	idx stepIndex;
public:
	ReadStep(Step& step_, idx step_index):step(step_),stepIndex(step_index)
	{
	}
	virtual ~ReadStep()
	{
	}
	virtual void process(InputContext &input);
	void setStepIndex(idx step_index)
	{
		stepIndex = step_index;
	}
};

} /* namespace RL */

#endif /* READSTEP_H_ */
