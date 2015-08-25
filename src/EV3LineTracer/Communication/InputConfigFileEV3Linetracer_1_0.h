/*
 * InputConfigFileEV3Linetracer_1_0.h
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#ifndef INPUTCONFIGFILEEV3LINETRACER_1_0_H_
#define INPUTCONFIGFILEEV3LINETRACER_1_0_H_


#include "../../Communication/InputContext.h"
#include "../../Communication/InputProcedure.h"
#include "../../Communication/Read/ReadRegularPolicy.h"
#include <string>
#include "../EV3LineTracer.h"
#include "Read/ReadInterval.h"
#include "Read/ReadStateCount.h"
#include "Read/ReadState.h"
#include "Read/ReadControl.h"
#include "Read/ReadCostMax.h"


namespace RL
{
// TODO クラス 「InputEV3Linetracer_1_0」がファイル読み込み専用のクラスになっているので
// EV3のバージョンを読み取る専用のものに改めること。
// ※「OutputEV3Linetracer_1_0」と揃えるため
class InputConfigFileEV3Linetracer_1_0: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	static const std::string VERSION_STRING;
	InputConfigFileEV3Linetracer_1_0(EV3LineTracer &ev3);
	virtual ~InputConfigFileEV3Linetracer_1_0();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* INPUTEV3LINETRACER_1_0_H_ */
