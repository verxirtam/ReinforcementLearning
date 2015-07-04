/*
 * ReadControl.h
 *
 *  Created on: 2015/01/25
 *      Author: daisuke
 */

#ifndef READSINGLECONTROL_H_
#define READSINGLECONTROL_H_
#include "../../../Communication/InputContext.h"
#include "../../../Communication/InputProcedure.h"
#include "../../../StringToData.h"
#include "../../EV3LineTracer.h"

namespace RL
{

class ReadSingleControl: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
	idx stateIndex;
	idx controlIndex;
public:
	ReadSingleControl(EV3LineTracer &ev3,idx si=0, idx ci=0);
	void setStateIndex(idx si)
	{
		stateIndex = si;
	}
	void setControlIndex(idx ci)
	{
		controlIndex = ci;
	}
	void setIndex(idx si, idx ci)
	{
		setStateIndex(si);
		setControlIndex(ci);
	}
	virtual ~ReadSingleControl();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* READCONTROL_H_ */
