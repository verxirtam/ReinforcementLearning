/*
 * InputEV3Linetracer_1_0.h
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#ifndef INPUTEV3LINETRACER_1_0_H_
#define INPUTEV3LINETRACER_1_0_H_


#include "../Communication/InputContext.h"
#include "../Communication/InputProcedure.h"
#include <string>
#include "EV3LineTracer.h"
#include "Communication/Read/ReadInterval.h"
#include "Communication/Read/ReadStateCount.h"
#include "Communication/Read/ReadState.h"
#include "Communication/Read/ReadControl.h"
#include "Communication/Read/ReadRegularPolicy.h"


namespace RL
{
// TODO クラス 「InputEV3Linetracer_1_0」がファイル読み込み専用のクラスになっているので
// EV3のバージョンを読み取る専用のものに改めること。
// ※「OutputEV3Linetracer_1_0」と揃えるため
class InputEV3Linetracer_1_0: public InputProcedure
{
private:
	EV3LineTracer& ev3LineTracer;
public:
	static const std::string VERSION_STRING;
	InputEV3Linetracer_1_0(EV3LineTracer &ev3);
	virtual ~InputEV3Linetracer_1_0();
	virtual void process(InputContext &input);
};

} /* namespace RL */

#endif /* INPUTEV3LINETRACER_1_0_H_ */
