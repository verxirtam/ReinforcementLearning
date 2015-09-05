/*
 * InputCommandExecEpisode.h
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#ifndef INPUTCOMMANDEXECEPISODE_H_
#define INPUTCOMMANDEXECEPISODE_H_

#include <iostream>
#include <sstream>
#include <string>

#include "../../Communication/InputContext.h"
#include "../../Communication/Read/ReadEpisode.h"

namespace RL
{

class InputCommandExecEpisode: public InputProcedure
{
private:
	Episode& episode;
	void processOnNG(InputContext& input);
public:
	InputCommandExecEpisode(Episode& episode_):episode(episode_)
	{
	}
	virtual ~InputCommandExecEpisode()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* INPUTCOMMANDEXECEPISODE_H_ */
