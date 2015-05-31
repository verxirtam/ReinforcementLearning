/*
 * ReadState.h
 *
 *  Created on: 2015/01/24
 *      Author: daisuke
 */

#ifndef READSINGLESTATE_H_
#define READSINGLESTATE_H_

#include "../RLUtility.h"
#include "../Communication/InputContext.h"
#include "../Communication/InputProcedure.h"
#include "../StringToData.h"
#include "EV3LineTracer.h"

namespace RL
{

class ReadSingleState: public InputProcedure
{
private:
	EV3LineTracer &ev3LineTracer;
	idx stateIndex;
public:
	ReadSingleState(EV3LineTracer &ev3,idx i = 0);
	void SetStateIndex(idx i)
	{
		stateIndex = i;
	}
	virtual ~ReadSingleState();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READSTATE_H_ */
