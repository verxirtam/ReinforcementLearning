/*
 * OutputEV3LineTracerConstructFile.h
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#ifndef OUTPUTEV3LINETRACERCONSTRUCTFILE_H_
#define OUTPUTEV3LINETRACERCONSTRUCTFILE_H_

#include "../../Communication/OutputProcedure.h"
#include "../EV3LineTracer.h"

namespace RL
{

class OutputEV3LineTracerConstructFile: public OutputProcedure
{
private:
	const EV3LineTracer& ev3LineTracer;
public:
	OutputEV3LineTracerConstructFile(const EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~OutputEV3LineTracerConstructFile()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTEV3LINETRACERCONSTRUCTFILE_H_ */
