/*
 * ReadCostMax.h
 *
 *  Created on: 2015/08/19
 *      Author: daisuke
 */

#ifndef READCOSTMAX_H_
#define READCOSTMAX_H_

#include <string>
#include "../../../Communication/InputProcedure.h"
#include "../../../Communication/InputContext.h"
#include "../../../RLUtility.h"
#include "../../../StringToData.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class ReadCostMax: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	ReadCostMax(EV3LineTracer &ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~ReadCostMax()
	{
	}
	virtual void process(InputContext &input);

};

} /* namespace RL */

#endif /* READCOSTMAX_H_ */
