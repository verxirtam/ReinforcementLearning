/*
 * ReadStep.h
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#ifndef READSTEP_H_
#define READSTEP_H_

#include <iostream>

#include "../../../Communication/InputContext.h"
#include "../../../Communication/InputProcedure.h"
#include "../../../Episode.h"
#include "../../../RLUtility.h"
#include "../../../StringToData.h"


namespace RL
{

class ReadStep: public InputProcedure
{
private:
	RL::Episode& episode;
	idx stepIndex;
public:
	ReadStep(RL::Episode& episode_, idx step_index):episode(episode_),stepIndex(step_index)
	{
	}
	virtual ~ReadStep()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READSTEP_H_ */
