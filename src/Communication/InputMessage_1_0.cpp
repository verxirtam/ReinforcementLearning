/*
 * InputMessage_1_0.cpp
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#include "InputMessage_1_0.h"

namespace RL
{

void InputMessage_1_0::process(InputContext &input)
{
	//メッセージバージョン
	input.skipToken("MESSAGE_1.0");
	input.skipReturn();

	//メッセージ本体
	inputProcedure.process(input);

	//メッセージ末尾の空行
	input.skipToken("");
}


} /* namespace RL */
