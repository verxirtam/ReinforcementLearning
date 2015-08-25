/*
 * WriteRegularPolicy.h
 *
 *  Created on: 2015/08/21
 *      Author: daisuke
 */

#ifndef WRITEREGULARPOLICY_H_
#define WRITEREGULARPOLICY_H_

#include "..//OutputProcedure.h"
#include "../../Policy.h"
#include "WritePolicy.h"

namespace RL
{

template <typename MDP>
class WriteRegularPolicy: public OutputProcedure
{
private:
	MDP& mdp;
public:
	WriteRegularPolicy(MDP& mdp_):mdp(mdp_)
	{
	}
	virtual ~WriteRegularPolicy()
	{
	}
	virtual void process(OutputContext& output)
	{
		Policy rp = mdp.getRegularPolicy();
		WritePolicy(rp).process(output);
	}

};

} /* namespace RL */

#endif /* WRITEREGULARPOLICY_H_ */
