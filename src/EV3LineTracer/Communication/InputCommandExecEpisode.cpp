/*
 * InputCommandExecEpisode.cpp
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#include "InputCommandExecEpisode.h"



namespace RL
{

void InputCommandExecEpisode::processOnNG(InputContext& input)
{
	std::ostringstream msg;
	msg << "ExecEpisode NG" << std::endl;
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

void InputCommandExecEpisode::process(InputContext &input)
{
	input.skipToken("ExecEpisode");
	input.skipReturn();
	std::string result = input.nextToken();
	input.skipReturn();
	if(result == "OK")
	{
		RL::ReadEpisode(episode).process(input);
		return;
	}
	else if(result == "NG")
	{
		processOnNG(input);
		return;
	}
	throw std::ios_base::failure("error on InputCommandExecEpisode::process().\n");
}

} /* namespace RL */
