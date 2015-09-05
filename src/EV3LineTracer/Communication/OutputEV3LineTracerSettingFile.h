/*
 * OutputEV3LineTracerSettingFile.h
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#ifndef OUTPUTEV3LINETRACERSETTINGFILE_H_
#define OUTPUTEV3LINETRACERSETTINGFILE_H_

#include <string>

#include "../../Communication/OutputContext.h"
#include "../../Communication/OutputProcedure.h"
#include "../EV3LineTracer.h"
#include "Write/WriteEV3LineTracerSetting.h"

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
	virtual void process(OutputContext& output);
};

} /* namespace RL */

#endif /* OUTPUTEV3LINETRACERSETTINGFILE_H_ */
