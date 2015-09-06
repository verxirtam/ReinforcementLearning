/*
 * ReadEV3LineTracerSetting.h
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#ifndef READEV3LINETRACERSETTING_H_
#define READEV3LINETRACERSETTING_H_

#include "../../../Communication/InputProcedure.h"
#include "../../../Communication/Read/ReadRegularPolicy.h"
#include "../../../Communication/Read/ReadValue.h"
#include "../../../Policy.h"
#include "../../EV3LineTracer.h"
#include "ReadControl.h"
#include "ReadDiscountRate.h"
#include "ReadCostMax.h"
#include "ReadInterval.h"
#include "ReadState.h"
#include "ReadStateCount.h"

namespace RL
{

class ReadEV3LineTracerSetting: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	ReadEV3LineTracerSetting(EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~ReadEV3LineTracerSetting()
	{
	}
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READEV3LINETRACERSETTING_H_ */
