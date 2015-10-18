/*
 * OutputPolicyEvaluationLogFile.h
 *
 *  Created on: 2015/09/10
 *      Author: daisuke
 */

#ifndef OUTPUTPOLICYEVALUATIONLOGFILE_H_
#define OUTPUTPOLICYEVALUATIONLOGFILE_H_


#include <vector>

#include "../RLUtility.h"
#include "OutputProcedure.h"
#include "../PolicyEvaluationStatistics.h"



namespace RL
{

class OutputPolicyEvaluationLogFile: public OutputProcedure
{
private:
	const RL::PolicyEvaluationStatistics& policyEvaluationStatistics;
	idx episodeIndex;
	real epsilon;
	std::vector<real> tDistribution95PercentPoint;
	std::vector<real> tDistribution99PercentPoint;
	idx tDistributionMaxIndex;
	void ConstructTDist(void);
public:
	OutputPolicyEvaluationLogFile(const RL::PolicyEvaluationStatistics& pes,
			idx k,real epsilon_) :
			policyEvaluationStatistics(pes), episodeIndex(k), epsilon(epsilon_), tDistribution95PercentPoint(), tDistribution99PercentPoint(),tDistributionMaxIndex(50)
	{
		ConstructTDist();
	}
	virtual ~OutputPolicyEvaluationLogFile()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTPOLICYEVALUATIONLOGFILE_H_ */
