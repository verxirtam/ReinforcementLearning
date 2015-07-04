/*
 * ReadRegularPolicy.h
 *
 *  Created on: 2015/05/31
 *      Author: daisuke
 */

#ifndef READREGULARPOLICY_H_
#define READREGULARPOLICY_H_

#include "../../../Communication/InputProcedure.h"
#include "../../../Communication/InputContext.h"
#include "../../../Policy.h"
#include "../../../RLUtility.h"
#include "../../../StringToData.h"
#include "../../EV3LineTracer.h"
#include "ReadSinglePolicy.h"


namespace RL
{

class ReadRegularPolicy: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	ReadRegularPolicy(EV3LineTracer& ev3);
	virtual ~ReadRegularPolicy();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READREGULARPOLICY_H_ */
