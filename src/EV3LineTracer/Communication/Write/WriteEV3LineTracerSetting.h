/*
 * WriteEV3LineTracerSetting.h
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#ifndef WRITEEV3LINETRACERSETTING_H_
#define WRITEEV3LINETRACERSETTING_H_

#include "../../../Communication/OutputProcedure.h"
#include "../../../Communication/Write/WriteRegularPolicy.h"
#include "../../../Communication/Write/WriteValue.h"
#include "../../EV3LineTracer.h"
#include "WriteControl.h"
#include "WriteCostMax.h"
#include "WriteInterval.h"
#include "WriteState.h"
#include "WriteStateCount.h"

namespace RL
{

class WriteEV3LineTracerSetting: public OutputProcedure
{
private:
	const EV3LineTracer& ev3LineTracer;
public:
	WriteEV3LineTracerSetting(const EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~WriteEV3LineTracerSetting()
	{
	}
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* EV3LINETRACERSETTING_H_ */
