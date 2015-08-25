/*
 * ReadRegularPolicy.h
 *
 *  Created on: 2015/05/31
 *      Author: daisuke
 */

#ifndef READREGULARPOLICY_H_
#define READREGULARPOLICY_H_

#include "../InputProcedure.h"
#include "ReadPolicy.h"


namespace RL
{
template <typename MDP>
class ReadRegularPolicy: public InputProcedure
{
private:
	MDP& mdp;
public:
	ReadRegularPolicy(MDP& mdp_) : mdp(mdp_)
	{
	}
	virtual ~ReadRegularPolicy()
	{
	}
	virtual void process(InputContext &input)
	{
		//読み取り用ポリシーの設定
		idx statecount = mdp.getStateCount();
		Policy p(statecount);

		ReadPolicy rp(p);

		//ポリシーの読み取り
		rp.process(input);

		//pがev3LineTracerと適合しているかチェックする
		p.verify(mdp);
		//ev3LineTracer.RegularPolicyへの書き込み
		mdp.setRegularPolicy(p);

	}
};



} /* namespace RL */

#endif /* READREGULARPOLICY_H_ */
