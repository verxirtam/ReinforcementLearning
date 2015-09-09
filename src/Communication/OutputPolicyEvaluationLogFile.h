/*
 * OutputPolicyEvaluationLogFile.h
 *
 *  Created on: 2015/09/10
 *      Author: daisuke
 */

#ifndef OUTPUTPOLICYEVALUATIONLOGFILE_H_
#define OUTPUTPOLICYEVALUATIONLOGFILE_H_


#include <string>
#include <vector>

#include "../EpsilonSoftOnPolicyMonteCarlo.h"
#include "../RLUtility.h"
#include "OutputContext.h"
#include "OutputProcedure.h"

namespace RL
{

class OutputPolicyEvaluationLogFile: public OutputProcedure
{
private:
	const RL::PolicyEvaluationStatistics& policyEvaluationStatistics;
public:
	OutputPolicyEvaluationLogFile(const RL::PolicyEvaluationStatistics& pes):policyEvaluationStatistics(pes)
	{
	}
	virtual ~OutputPolicyEvaluationLogFile()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTPOLICYEVALUATIONLOGFILE_H_ */
