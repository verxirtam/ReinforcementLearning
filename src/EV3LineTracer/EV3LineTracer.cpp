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
#include "Communication/InputCommandSetCurrentPolicy.h"
#include "Communication/OutputCommandNullCommand.h"
#include "Communication/OutputCommandSetMDP.h"
#include "Communication/OutputCommandExecEpisode.h"
#include "Communication/OutputCommandSetCurrentPolicy.h"
#include "Communication/OutputEV3LineTracerConstructFile.h"
#include "Communication/OutputEV3LineTracerSettingFile.h"
#include "Communication/OutputEV3LineTracerEpisodeLogFile.h"
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

	//設定をファイル出力する
	if(loggingEnable)
	{
		setSettingTime();
		writeEV3LineTracerSettingFile();
	}
}

//コマンドの実行
void EV3LineTracer::execCommand(RL::OutputProcedure& o_command,
		RL::InputProcedure& i_command)const
{
	//EV3への接続を確立
	TCPClient tcp_client("localhost", 50000, 1024);//"192.168.0.11",50000,1024);//
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
bool EV3LineTracer::getEpisode(Episode& episode)const
{
	//このコマンド用のInput、Output Procedure
	RL::OutputCommandExecEpisode o_exec_episode;
	RL::InputCommandExecEpisode i_exec_episode(episode);

	std::time_t start_time;
	if(loggingEnable)
	{
		std::time(&start_time);
	}

	//コマンド実行
	this->execCommand(o_exec_episode, i_exec_episode);

	if(loggingEnable)
	{
		std::time_t finish_time;
		std::time(&finish_time);

		writeEpisodeLogFile(start_time,finish_time,episode);
	}

	return true;
}


//CurrentPolicyの設定
void EV3LineTracer::setCurrentPolicy(const StochasticPolicy& p)
{
	//データメンバのcurrentPolicyを更新する
	setCurrentPolicyLocal(p);

	//EV3へのコマンドを作成
	OutputCommandSetCurrentPolicy ocscp(*this);
	InputCommandSetCurrentPolicy icscp(*this);

	//EV3へのコマンド「SetCurrentPolicy」を作成
	this->execCommand(ocscp,icscp);

}
std::string EV3LineTracer::getConstructTimeString()const
{
	return RL::TimeToString::toString(constructTime);
}

void EV3LineTracer::writeFile(const std::string& logfilepath,
		RL::OutputProcedure& output)const
{
	std::ofstream ofs(logfilepath);
	//ファイルオープンの確認
	if (!ofs)
	{
		std::string msg("EV3LineTracer::writeFile() : can not open ");
		msg += logfilepath;
		throw std::ios_base::failure(msg);
	}
	try
	{
		RL::TSVOutputContext toc(ofs);
		output.process(toc);
		ofs.close();
	} catch (...)
	{
		ofs.close();
		throw;
	}
}

void EV3LineTracer::writeConstructFile(void)const
{
	//ファイルパスの作成
	std::stringstream logfilepath("");
	logfilepath << this->logDirectoryPath;
	logfilepath << "EV3LineTracer_";
	logfilepath << TimeToString::toStringForFileName(this->constructTime);
	logfilepath << "_00Construct";
	logfilepath << ".log";


	//出力用OutputProcedureを初期化
	RL::OutputEV3LineTracerConstructFile output(*this);

	writeFile(logfilepath.str(), output);
}
void EV3LineTracer::writeDestructFile(void)const
{
	time_t destruct_time;
	std::time(&destruct_time);

	//ファイルパスの作成
	std::stringstream logfilepath("");
	logfilepath << this->logDirectoryPath;
	logfilepath << "EV3LineTracer_";
	logfilepath << TimeToString::toStringForFileName(this->constructTime);
	logfilepath << "_99Destruct_";
	logfilepath << TimeToString::toStringForFileName(destruct_time);
	logfilepath << ".log";


	//出力用OutputProcedureを初期化
	RL::OutputEV3LineTracerDestructFile output(destruct_time);

	writeFile(logfilepath.str(), output);
}
void EV3LineTracer::writeEV3LineTracerSettingFile(void)
{
	//ファイルパスの作成
	std::stringstream logfilepath("");
	logfilepath << this->logDirectoryPath;
	logfilepath << "EV3LineTracer_";
	logfilepath << TimeToString::toStringForFileName(this->constructTime);
	logfilepath << "_10Setting_";
	logfilepath << TimeToString::toStringForFileName(this->settingTime);
	logfilepath << ".log";

	//ファイルパスを保存
	settingFilePath=logfilepath.str();

	//出力用OutputProcedureを初期化
	RL::OutputEV3LineTracerSettingFile output(*this);

	writeFile(logfilepath.str(), output);

}

void EV3LineTracer::writeEpisodeLogFile(const std::time_t& start_time,const std::time_t&finish_time,const Episode& episode)const
{
	//ファイルパスの作成
	std::stringstream logfilepath("");
	logfilepath << this->logDirectoryPath;
	logfilepath << "EV3LineTracer_";
	logfilepath << TimeToString::toStringForFileName(this->constructTime);
	logfilepath << "_20Episode_";
	logfilepath << TimeToString::toStringForFileName(start_time);
	logfilepath << ".log";


	//出力用OutputProcedureを初期化
	RL::OutputEV3LineTracerEpisodeLogFile output(*this,start_time,finish_time,episode);

	writeFile(logfilepath.str(), output);

}

}


