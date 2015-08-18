/*
 * EV3LineTracer.cpp
 *
 *  Created on: 2014/01/24
 *      Author: daisuke
 */

#include "EV3LineTracer.h"
#include "Communication/InputConfigFileEV3Linetracer_1_0.h"//相互参照防止
#include "Communication/InputConfigFile.h"//相互参照防止




using namespace std;

namespace RL
{



//設定ファイル読み込みを行う
void EV3LineTracer::readConfigFile()
{

	//読み込み用に設定ファイルを開く
	std::ifstream ifs(configFilePath.c_str());
	//ファイルオープンの確認
	if(!ifs)
	{
		std::string msg("EV3LineTracer::ReadConfigFile() : can not open ");
		msg+=configFilePath;
		throw std::ios_base::failure(msg);
	}
	try
	{
		//タブ区切りの入力コンテキスト
		RL::TSVInputContext tic(ifs);

		RL::InputConfigFile icf(*this);
		icf.process(tic);

		ifs.close();
	}
	catch(...)
	{
		ifs.close();
		throw;
	}
}

//EV3の準備を行う
void EV3LineTracer::initEV3()
{
	//EV3と通信できるか確認する
	this->execNullCommand();
	// TODO CostMaxを取得する処理を書いていたが、INIファイルから取得するように改める
}

//EV3LineTracer::Init()
//設定ファイル読み込みを行い、パラメータを初期化する
void EV3LineTracer::init()
{
	//設定ファイル読み込みを行う
	readConfigFile();

	//EV3の準備を行う
	this->initEV3();

}
void EV3LineTracer::execNullCommand()
{
	//EV3への接続を確立
	TCPClient tcp_client("localhost",50000,1024);//"192.168.0.8",50000,1024));

	//EV3への送信用のデータ(NULLコマンド)を作成
	RL::OutputCommandNullCommand o_null_command;
	RL::OutputEV3LineTracer_1_0 o_ev3_1_0(o_null_command);
	RL::OutputMessage_1_0 o_message(o_ev3_1_0);

	//EV3へ送信
	OutputContext &oc = tcp_client.getOutputContext();
	o_message.process(oc);

	//EV3からの返信を受信する
	InputContext &ic = tcp_client.getInputContext();

	//メッセージ受信用のInputProcedure
	RL::InputCommandNullCommand i_null_command;
	RL::InputEV3LineTracer_1_0 i_ev3_1_0(i_null_command);
	RL::InputMessage_1_0 i_message(i_ev3_1_0);

	i_message.process(ic);
}
//MDPの設定をEV3に反映する
void EV3LineTracer::execSetMDP()
{
	//EV3への接続を確立
	TCPClient tcp_client("localhost",50000,1024);//"192.168.0.8",50000,1024));

	//EV3への送信用のデータ(NULLコマンド)を作成
	RL::OutputCommandSetMDP o_set_mdp;
	RL::OutputEV3LineTracer_1_0 o_ev3_1_0(o_set_mdp);
	RL::OutputMessage_1_0 o_message(o_ev3_1_0);

	//EV3へ送信
	OutputContext &oc = tcp_client.getOutputContext();
	o_message.process(oc);

	//EV3からの返信を受信する
	InputContext &ic = tcp_client.getInputContext();

	//メッセージ受信用のInputProcedure
	RL::InputCommandSetMDP i_set_mdp;
	RL::InputEV3LineTracer_1_0 i_ev3_1_0(i_set_mdp);
	RL::InputMessage_1_0 i_message(i_ev3_1_0);

	i_message.process(ic);
}



}


