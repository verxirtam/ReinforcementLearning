/*
 * InputCommandSetCurrentPolicy.cpp
 *
 *  Created on: 2015/08/30
 *      Author: daisuke
 */

#include "InputCommandSetCurrentPolicy.h"


namespace RL
{
void InputCommandSetCurrentPolicy::processOnNG(InputContext& input)
{
	std::ostringstream msg;
	msg << "SetCurrentPolicy NG" << std::endl;
	while (true)
	{
		if (!input.hasNextToken())
		{
			input.skipReturn();
			msg << std::endl;
		}
		std::string token = input.nextToken();
		if (token == "")
		{
			break; //while
		}
		msg << token;
		msg << '\t';
	}
	throw std::ios_base::failure(msg.str());
}
void InputCommandSetCurrentPolicy::process(InputContext &input)
{
	input.skipToken("SetCurrentPolicy");
	input.skipReturn();
	std::string result = input.nextToken();
	input.skipReturn();
	if(result == "OK")
	{
		StochasticPolicy sp(ev3LineTracer);
		ReadStochasticPolicy(sp).process(input);
		if (ev3LineTracer.getCurrentPolicy().isFar(sp))
		{
			throw std::ios_base::failure("SetCurrentPolicyに失敗しました");
		}
		return;
	}
	processOnNG(input);
}

} /* namespace RL */
