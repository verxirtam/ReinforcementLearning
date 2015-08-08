/*
 * EV3LineTracer.cpp
 *
 *  Created on: 2014/01/24
 *      Author: daisuke
 */

#include "EV3LineTracer.h"

#include <iostream>//TODO

#include "../Communication/TSVInputContext.h"
#include "InputConfigFile.h"
#include "InputEV3Linetracer_1_0.h"

using namespace std;

namespace RL
{



//設定ファイル読み込みを行う
void EV3LineTracer::ReadConfigFile()
{

	//読み込み用に設定ファイルを開く
	std::ifstream ifs(ConfigFilePath.c_str());
	//ファイルオープンの確認
	if(!ifs)
	{
		std::string msg("EV3LineTracer::ReadConfigFile() : can not open ");
		msg+=ConfigFilePath;
		throw std::ios_base::failure(msg);
	}
	try
	{
		//タブ区切りの入力コンテキスト
		RL::TSVInputContext tic(ifs);

		RL::InputEV3Linetracer_1_0 iev3(*this);
		RL::InputConfigFile icf(iev3);
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
void EV3LineTracer::InitEV3()
{
	//EV3への接続を確立
	tcpClient=auto_ptr<TCPClient>(new TCPClient("localhost",50000,1024));
	//TODO MDP(State[], Control[][])を送信(CostMaxは-1として送信)
	//TODO CurrentPolicyをRegularPolicyとする
	//TODO EV3に1-Episode実行させる(GetEpisode()を実行)
	//TODO Episodeを取得し、完走できたか確認(Costが-1かどうかで判断)し、成功するまで再実行する
	//TODO 成功したEpisodeのCostを取得し、CostMaxを取得したCostの10倍として定め、EV3にCostMaxを送信する
	//TODO (Episodeを終了した時点でEV3はStart地点で待機している)
}

//EV3LineTracer::Init()
//設定ファイル読み込みを行い、パラメータを初期化する
void EV3LineTracer::Init()
{
	//設定ファイル読み込みを行う
	ReadConfigFile();

	//EV3の準備を行う
	this->InitEV3();

}
void EV3LineTracer::execNullCommand()
{
	//throw std::ios::failure("未実装");
	//EV3への送信用のデータ(NULLコマンド)を作成
	stringstream null_command_string("");
	null_command_string<<"MESSAGE_1.0"<<endl;
	null_command_string<<"EV3LineTracer_1.0"<<endl;
	null_command_string<<"NullCommand"<<endl;
	null_command_string<<endl;

	//EV3へ送信
	OutputContext &oc = tcpClient->getOutputContext();
	oc.writeToken(null_command_string.str());
	oc.flush();

	//TODO EV3からの返信を受信する
	InputContext &ic = tcpClient->getInputContext();
	string s[4];
	s[0] = ic.nextToken();ic.skipReturn();
	s[1] = ic.nextToken();ic.skipReturn();
	s[2] = ic.nextToken();ic.skipReturn();
	s[3] = ic.nextToken();ic.skipReturn();

	if(
		!(
			s[0]==string("MESSAGE_1.0")
			&& s[1]==string("EV3LineTracer_1.0")
			&& s[2]==string("NullCommand")
			&& s[3]==string("OK")
		)
	)
	{
		throw std::ios::failure("返信メッセージが不正です");
	}

	//TODO 結果を読み取る
	//TODO 結果に応じた処理を実行する
}


}


