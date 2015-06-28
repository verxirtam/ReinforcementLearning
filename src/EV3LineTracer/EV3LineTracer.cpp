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
	/*
	std::ifstream fin(ConfigFilePath.c_str());
	//ファイルオープンの確認
	if(!fin)
	{
		std::string msg("EV3LineTracer::Init() : can not open ");
		msg+=ConfigFilePath;
		throw std::ios_base::failure(msg);
	}
	//フォーマット識別子の確認
	this->CheckFormatIdentifier(fin);
	//設定の取得
	//Intervalの設定
	this->ReadInterval(fin);
	//StateCountの設定
	this->ReadStateCount(fin);
	//StateとControlCountの設定
	this->ReadStateAndControlCount(fin);
	//Controlの設定
	this->ReadControl(fin);
	//RegularPolicyの設定
	this->ReadRegularPolicy(fin);
	//入力ファイルをクローズ
	fin.close();
	*/
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
	//TODO EV3への接続を確立
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


}


