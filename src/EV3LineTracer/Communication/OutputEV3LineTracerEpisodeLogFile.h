/*
 * OutputEV3LineTracerEpisodeLogFile.h
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#ifndef OUTPUTEV3LINETRACEREPISODELOGFILE_H_
#define OUTPUTEV3LINETRACEREPISODELOGFILE_H_

#include <ctime>
#include <string>

#include "../../Communication/OutputProcedure.h"
#include "../../Communication/OutputContext.h"
#include "../../Communication/Write/WriteCurrentPolicy.h"
#include "../../Communication/Write/WriteEpisode.h"
#include "../../TimeToString.h"
#include "../../Episode.h"
#include "../EV3LineTracer.h"

namespace RL
{

class OutputEV3LineTracerEpisodeLogFile: public OutputProcedure
{
private:
	const EV3LineTracer& ev3LineTracer;
	const time_t startTime;
	const time_t finishTime;
	const Episode& episode;
public:
	OutputEV3LineTracerEpisodeLogFile(const EV3LineTracer& ev3,
			const time_t start_time, const time_t finish_time,
			const Episode& episode_) :
				ev3LineTracer(ev3),
				startTime(start_time),
				finishTime(finish_time),
				episode(episode_)
	{
	}
	virtual ~OutputEV3LineTracerEpisodeLogFile()
	{
	}
	virtual void process(OutputContext& output);

};

} /* namespace RL */

#endif /* OUTPUTEV3LINETRACEREPISODELOGFILE_H_ */
