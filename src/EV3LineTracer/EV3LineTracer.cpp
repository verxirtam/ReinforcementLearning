/*
 * EV3LineTracer.cpp
 *
 *  Created on: 2014/01/24
 *      Author: daisuke
 */

#include "EV3LineTracer.h"
//↓相互参照防止
#include "Communication/InputConfigFileEV3Linetracer_1_0.h"
#include "Communication/InputConfigFile.h"
#include "Communication/InputCommandNullCommand.h"
#include "Communication/InputCommandSetMDP.h"
#include "Communication/InputCommandExecEpisode.h"
#include "Communication/OutputCommandNullCommand.h"
#include "Communication/OutputCommandSetMDP.h"
#include "Communication/OutputCommandExecEpisode.h"
//↑相互参照防止




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

	//MDPの設定を送信する
	this->execSetMDP();

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

//コマンドの実行
void EV3LineTracer::execCommand(RL::OutputProcedure& o_command,
		RL::InputProcedure& i_command)
{
	//EV3への接続を確立
	TCPClient tcp_client("192.168.0.7",50000,1024);//"localhost", 50000, 1024);
	//EV3への送信用のデータを作成
	RL::OutputEV3LineTracer_1_0 o_ev3_1_0(o_command);
	RL::OutputMessage_1_0 o_message(o_ev3_1_0);
	//EV3へ送信
	OutputContext& oc = tcp_client.getOutputContext();
	o_message.process(oc);
	//EV3からの返信を受信する
	InputContext& ic = tcp_client.getInputContext();
	//メッセージ受信用のInputProcedure
	RL::InputEV3LineTracer_1_0 i_ev3_1_0(i_command);
	RL::InputMessage_1_0 i_message(i_ev3_1_0);
	//受信結果に応じた処理
	i_message.process(ic);
}

//何もしないコマンド（通信の疎通確認等に使用する）
void EV3LineTracer::execNullCommand()
{

	//このコマンド用のInput、Output Procedure
	RL::OutputCommandNullCommand o_null_command;
	RL::InputCommandNullCommand i_null_command;

	//コマンド実行
	execCommand(o_null_command, i_null_command);
}

//MDPの設定をEV3に反映する
void EV3LineTracer::execSetMDP()
{
	//このコマンド用のInput、Output Procedure
	RL::OutputCommandSetMDP o_set_mdp(*this);
	RL::InputCommandSetMDP i_set_mdp(*this);

	//コマンド実行
	execCommand(o_set_mdp, i_set_mdp);
}
//エピソードを取得する
bool EV3LineTracer::getEpisode(Episode& episode)
{
	//このコマンド用のInput、Output Procedure
	RL::OutputCommandExecEpisode o_exec_episode;
	RL::InputCommandExecEpisode i_exec_episode(episode);

	//コマンド実行
	this->execCommand(o_exec_episode, i_exec_episode);

	return true;
}

}


