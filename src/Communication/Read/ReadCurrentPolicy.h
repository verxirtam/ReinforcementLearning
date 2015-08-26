/*
 * ReadCurrentPolicy.h
 *
 *  Created on: 2015/08/27
 *      Author: daisuke
 */

#ifndef READCURRENTPOLICY_H_
#define READCURRENTPOLICY_H_

namespace RL
{
template <typename MDP>
class ReadCurrentPolicy: public InputProcedure
{
private:
	MDP& mdp;
public:
	ReadCurrentPolicy(MDP& mdp_):mdp(mdp_)
	{
	}
	virtual ~ReadCurrentPolicy()
	{
	}
	virtual void process(InputContext &input)
	{
		StochasticPolicy cp(mdp);
		ReadStochasticPolicy rsp(cp);
		rsp.process(input);
		mdp.setCurrentPolicy(cp);
	}

};

} /* namespace RL */

#endif /* READCURRENTPOLICY_H_ */
