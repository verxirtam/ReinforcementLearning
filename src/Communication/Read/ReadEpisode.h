/*
 * ReadEpisode.h
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#ifndef READEPISODE_H_
#define READEPISODE_H_


#include "../../Episode.h"
#include "../../RLUtility.h"
#include "../../StringToData.h"
#include "../InputProcedure.h"
#include "../InputContext.h"
#include "ReadStep.h"

namespace RL
{

class ReadEpisode: public InputProcedure
{
private:
	Episode& episode;
public:
	ReadEpisode(RL::Episode& episode_):episode(episode_)
	{
	}
	virtual ~ReadEpisode()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READEPISODE_H_ */
