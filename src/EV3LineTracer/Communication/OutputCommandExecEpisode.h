/*
 * OutputCommandExecEpisode.h
 *
 *  Created on: 2015/08/20
 *      Author: daisuke
 */

#ifndef OUTPUTCOMMANDEXECEPISODE_H_
#define OUTPUTCOMMANDEXECEPISODE_H_

#include "../../Communication/OutputProcedure.h"
#include "../../Communication/OutputContext.h"

namespace RL
{

class OutputCommandExecEpisode: public OutputProcedure
{
public:
	OutputCommandExecEpisode()
	{
	}
	virtual ~OutputCommandExecEpisode()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTCOMMANDEXECEPISODE_H_ */
