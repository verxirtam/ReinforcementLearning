/*
 * InputConfigFile.cpp
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#include "InputConfigFile.h"

namespace RL
{


void InputConfigFile::process(InputContext &input)
{
	//ファイル一行目のバージョン文字列を取得
	std::string version = input.nextToken();
	input.skipReturn();

	//バージョン文字列に応じたInputProcedureを実行する
	if(version == InputConfigFileEV3Linetracer_1_0::VERSION_STRING)
	{
		InputConfigFileEV3Linetracer_1_0 body(ev3LineTracer);
		body.process(input);
	}
	else
	{
		//バージョンが一致しなかったら例外を投げる
		throw  std::ios::failure("バージョンが不正です");
	}
}

} /* namespace RL */
