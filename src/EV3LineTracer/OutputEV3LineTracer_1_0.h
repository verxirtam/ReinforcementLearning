/*
 * OutputEV3LineTracer10.h
 *
 *  Created on: 2015/08/17
 *      Author: daisuke
 */

#ifndef OUTPUTEV3LINETRACER10_H_
#define OUTPUTEV3LINETRACER10_H_

#include "../Communication/OutputProcedure.h"

namespace RL
{

class OutputEV3LineTracer_1_0: public OutputProcedure
{
private:
	OutputProcedure& output;
public:
	OutputEV3LineTracer_1_0(OutputProcedure& o):output(o)
	{
	}
	virtual ~OutputEV3LineTracer_1_0()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTEV3LINETRACER10_H_ */
