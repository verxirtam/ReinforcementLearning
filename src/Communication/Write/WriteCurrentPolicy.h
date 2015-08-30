/*
 * WriteCurrentPolicy.h
 *
 *  Created on: 2015/08/29
 *      Author: daisuke
 */

#ifndef WRITECURRENTPOLICY_H_
#define WRITECURRENTPOLICY_H_

#include "../../Policy.h"
#include "WriteStochasticPolicy.h"


namespace RL
{

template<typename MDP>
class WriteCurrentPolicy: public OutputProcedure
{
private:
	const MDP& mdp;
public:
	WriteCurrentPolicy(const MDP& mdp_):mdp(mdp_)
	{
	}
	virtual ~WriteCurrentPolicy()
	{
	}
	virtual void process(OutputContext& output)
	{
		StochasticPolicy sp(mdp.getCurrentPolicy());
		WriteStochasticPolicy rsp(sp);
		rsp.process(output);
	}
};

} /* namespace RL */

#endif /* WRITECURRENTPOLICY_H_ */
