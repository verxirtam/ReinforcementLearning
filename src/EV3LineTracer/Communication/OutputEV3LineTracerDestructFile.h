/*
 * OutputEV3LineTracerDestructFile.h
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#ifndef OUTPUTEV3LINETRACERDESTRUCTFILE_H_
#define OUTPUTEV3LINETRACERDESTRUCTFILE_H_

#include <ctime>

#include "../../Communication/OutputProcedure.h"
#include "../../Communication/OutputContext.h"
#include "../../TimeToString.h"

namespace RL
{

class OutputEV3LineTracerDestructFile: public OutputProcedure
{
private:
	const time_t& destructTime;
public:
	OutputEV3LineTracerDestructFile(const time_t& dest_time):destructTime(dest_time)
	{
	}
	virtual ~OutputEV3LineTracerDestructFile()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTEV3LINETRACERDESTRUCTFILE_H_ */
