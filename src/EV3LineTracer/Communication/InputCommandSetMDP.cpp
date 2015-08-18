/*
 * InputCommandSetMDP.cpp
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#include "InputCommandSetMDP.h"

namespace RL
{
void InputCommandSetMDP::process(InputContext &input)
{
	input.skipToken("SetMDP");
	input.skipReturn();
	std::string result = input.nextToken();
	input.skipReturn();
	if(result != "OK")
	{

		std::ostringstream msg;
		msg << "SetMDP NG" << std::endl;
		while(true)
		{
			if(!input.hasNextToken())
			{
				input.skipReturn();
				msg << std::endl;
			}
			std::string token = input.nextToken();
			if(token == "")
			{
				break;//while
			}
			msg << token;
			msg << '\t';
		}
		throw std::ios_base::failure(msg.str());
	}
}
} /* namespace RL */
