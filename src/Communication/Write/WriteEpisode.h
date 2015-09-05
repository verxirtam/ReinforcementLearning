/*
 * WriteEpisode.h
 *
 *  Created on: 2015/09/05
 *      Author: daisuke
 */

#ifndef WRITEEPISODE_H_
#define WRITEEPISODE_H_

#include "../../Communication/OutputProcedure.h"
#include "../../Episode.h"

namespace RL
{

class WriteEpisode: public OutputProcedure
{
	const Episode& episode;
public:
	WriteEpisode(const Episode& episode_):episode(episode_)
	{
	}
	virtual ~WriteEpisode()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* WRITEEPISODE_H_ */
