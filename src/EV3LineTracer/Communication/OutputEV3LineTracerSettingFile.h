/*
 * OutputEV3LineTracerSettingFile.h
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#ifndef OUTPUTEV3LINETRACERSETTINGFILE_H_
#define OUTPUTEV3LINETRACERSETTINGFILE_H_

#include <string>

#include "../../Communication/OutputProcedure.h"
#include "../../Communication/OutputContext.h"
#include "../../Communication/Write/WriteRegularPolicy.h"
#include "../../Communication/Write/WriteValue.h"
#include "../EV3LineTracer.h"
#include "Write/WriteControl.h"
#include "Write/WriteCostMax.h"
#include "Write/WriteInterval.h"
#include "Write/WriteState.h"
#include "Write/WriteStateCount.h"

namespace RL
{

class OutputEV3LineTracerSettingFile: public OutputProcedure
{
private:
	const EV3LineTracer& ev3LineTracer;
public:
	OutputEV3LineTracerSettingFile(const EV3LineTracer& ev3):ev3LineTracer(ev3)
	{
	}
	virtual ~OutputEV3LineTracerSettingFile()
	{
	}
	void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTEV3LINETRACERSETTINGFILE_H_ */
