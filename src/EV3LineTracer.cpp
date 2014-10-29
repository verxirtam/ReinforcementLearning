/*
 * EV3LineTracer.cpp
 *
 *  Created on: 2014/01/24
 *      Author: daisuke
 */

#include "EV3LineTracer.h"

#include <iostream>//TODO

using namespace std;

namespace RL
{

const string EV3LineTracer::ELEMENTNAME_STATE="State";
const string EV3LineTracer::ELEMENTNAME_STATEINDEX="StateIndex";
const string EV3LineTracer::ELEMENTNAME_STATECOUNT="StateCount";
const string EV3LineTracer::ELEMENTNAME_STATE_REFMAX="State.RefMax";
const string EV3LineTracer::ELEMENTNAME_CONTROL="Control";
const string EV3LineTracer::ELEMENTNAME_CONTROLINDEX="ControlIndex";
const string EV3LineTracer::ELEMENTNAME_CONTROL_LMOTORSPEED="Control.LMotorSpeed";
const string EV3LineTracer::ELEMENTNAME_CONTROL_RMOTORSPEED="Control.RMotorSpeed";
const string EV3LineTracer::ELEMENTNAME_CONTROLCOUNT="ControlCount";
const string EV3LineTracer::ELEMENTNAME_REGULARPOLICY="RegularPolicy";

//設定ファイルの1行分読み込み
void EV3LineTracer::ReadLine(std::ifstream& fin,std::istringstream& linestream)
{
	//一行のファイル内容を格納
	string line;
	//コメントを除いた行
	string nocomentline;
	//設定ファイルから1行取得
	getline(fin,line);
	//読み込みエラーの場合は例外を投げる
	if(!fin)
	{
		std::string msg("invalid file format ( can not read line ) : ");
		msg+=ConfigFilePath;
		throw std::ios_base::failure(msg);
	}
	//コメントを削除する
	DeleteComment(line,nocomentline);
	//linestreamのバッファにlineを設定する
	linestream.str(nocomentline);
}

//1行分のstringからコメント(";"の手前のWSから行末まで)を削除
void EV3LineTracer::DeleteComment(const std::string& line, std::string& out)
{
	uint i;
	//";"までの文字数を取得
	for(i=0;i<line.length();i++)
	{
		if(line[i]==';')
		{
			break;
		}
	}
	//iを";"(または行末)の1つ手前にする
	i--;
	//";"の手前のWSの1つ手前の位置を取得
	for(;i>=0;i--)
	{
		if((line[i]!=' ')&&(line[i]!='\t'))
		{
			break;
		}
	}
	//コメントが削除された行の文字数を取得
	int len=i+1;
	//コメントが削除された行を取得
	out=line.substr(0,len);
}


void EV3LineTracer::CheckFormatIdentifier(std::ifstream& fin)
{
	//設定ファイルから読み取った1行に対応するstring stream
	istringstream linestream;
	//設定ファイルからの1行読み取り
	ReadLine(fin,linestream);

	//読み取ったfiletype
	std::string filetype=linestream.str();
	if(filetype!="EV3LineTracer_1.0")
	{
		std::string msg("EV3LineTracer::Init() : invalid file format (filetype) : ");
		msg+=ConfigFilePath;
		throw std::ios_base::failure(msg);
	}

}
//行内に余計な情報が残っていないかをチェックする
void EV3LineTracer::CheckUnnecessaryData(istringstream& linestream)
{
	//Stateの行に余計な情報が入っている場合
	if(!linestream.eof())
	{
		std::string msg("invalid file format (unnecessary data) : ");
		msg+=ConfigFilePath;
		throw std::ios_base::failure(msg);
	}
}
//設定ファイル読み込み Interval取得
void EV3LineTracer::ReadInterval(std::ifstream& fin)
{
	//設定ファイルから読み取った1行に対応するstring stream
	istringstream linestream;
	//設定ファイルからの1行読み取り
	ReadLine(fin,linestream);

	//トークンの変数への読み込み
	ReadToken(linestream,ELEMENTNAME_STATECOUNT,Interval);
	//StateCountが不正な値(<=1)の時は例外を投げる
	if(Interval<1)
	{
		std::string msg("EV3LineTracer::ReadStateCount() : invalid value (Interval<1) : ");
		msg+=ConfigFilePath;
		throw std::ios_base::failure(msg);
	}
	//StateCountの行に余計な情報が入っている場合は例外を投げる
	this->CheckUnnecessaryData(linestream);
	//この時点でStateCountは正常な値(>=1)として読み取れた
}


//設定ファイル読み込み StateCount取得
void EV3LineTracer::ReadStateCount(std::ifstream& fin)
{
	//設定ファイルから読み取った1行に対応するstring stream
	istringstream linestream;
	//設定ファイルからの1行読み取り
	ReadLine(fin,linestream);

	//トークンの変数への読み込み
	ReadToken(linestream,ELEMENTNAME_STATECOUNT,StateCount);
	//StateCountが不正な値(<=1)の時は例外を投げる
	if(StateCount<=1)
	{
		std::string msg("EV3LineTracer::ReadStateCount() : invalid value (StateCount<=1) : ");
		msg+=ConfigFilePath;
		throw std::ios_base::failure(msg);
	}
	//StateCountの行に余計な情報が入っている場合は例外を投げる
	this->CheckUnnecessaryData(linestream);
	//この時点でStateCountは正常な値(>1)として読み取れた
}
//設定ファイル読み込み StateとControlCount取得
void EV3LineTracer::ReadStateAndControlCount(std::ifstream& fin)
{
	//vector Stateのサイズを定める
	State.resize(StateCount);
	//vector ControlCountのサイズを定める
	ControlCount.resize(StateCount);

	//StateCount回繰り替えしてStateの情報取得
	for(idx i=0;i<StateCount;i++)
	{

		//設定ファイルから読み取った1行に対応するstring stream
		istringstream linestream;
		//設定ファイルからの1行読み取り
		ReadLine(fin,linestream);

		//読み取ったstateのindex
		idx readi;
		//state indexの読み取り
		ReadToken(linestream,ELEMENTNAME_STATEINDEX,readi);
		if(readi!=i)
		{
			std::string msg("invalid file format (invalid State Index) : ");
			msg+=ConfigFilePath;

			throw std::ios_base::failure(msg);
		}
		//RefMaxの読み取り
		ReadToken(linestream,ELEMENTNAME_STATE,State[i].RefMax);
		//ControlCountの読み取り
		ReadToken(linestream,ELEMENTNAME_CONTROLCOUNT,ControlCount[i]);
		//Stateの行に余計な情報が入っている場合は例外を投げる
		this->CheckUnnecessaryData(linestream);
	}
	//State[]正当性チェック
	for(idx i=0;i<StateCount;i++)
	{
		//State[i].RefMaxのチェック
		//値が0以下の場合は例外を投げる
		if(State[i].RefMax<=0.0)
		{
			std::string msg("invalid data ("+ELEMENTNAME_STATE_REFMAX+") : ");
			msg+=ConfigFilePath;
			throw std::ios_base::failure(msg);
		}
		//State[i].RefMax<=State[i-1].RefMaxの場合は例外を投げる
		if(i>0)
		{
			if(State[i].RefMax<=State[i-1].RefMax)
			{
				std::string msg("invalid data ("+ELEMENTNAME_STATE_REFMAX+") : ");
				msg+=ConfigFilePath;
				throw std::ios_base::failure(msg);
			}
		}
	}
	if(State[StateCount-1].RefMax!=1.0)
	{
		std::string msg("invalid data ("+ELEMENTNAME_STATE_REFMAX+") : ");
		msg+=ConfigFilePath;
		throw std::ios_base::failure(msg);
	}
	//ControlCount[]のチェック
	if(ControlCount[0]!=1)
	{
		std::string msg("invalid data ("+ELEMENTNAME_CONTROLCOUNT+":ControlCount[0]!=1) : ");
		msg+=ConfigFilePath;
		throw std::ios_base::failure(msg);
	}


}
//設定ファイル読み込み Control取得
void EV3LineTracer::ReadControl(std::ifstream& fin)
{
	//Controlのサイズを設定する
	Control.resize(StateCount);
	//StateCount回繰り替えしStateごとに読み込む
	for(idx i=0;i<StateCount;i++)
	{
		//ControlCount[i]の取得
		const idx controlcount=ControlCount[i];
		//Control[i]のサイズの設定
		Control[i].resize(controlcount);
		for(idx u=0;u<controlcount;u++)
		{
			//設定ファイルから読み取った1行に対応するstring stream
			istringstream linestream;
			//設定ファイルからの1行読み取り
			ReadLine(fin,linestream);

			//読み取ったstateのindex
			idx readi;
			//state indexの読み取り
			ReadToken(linestream,ELEMENTNAME_STATEINDEX,readi);
			//読み取った値がiと異なる場合は例外を投げる
			if(readi!=i)
			{
				std::string msg("invalid file format (invalid State Index) : ");
				msg+=ConfigFilePath;
				throw std::ios_base::failure(msg);
			}

			//読み取ったcontrolのindex
			idx readu;
			//control indexの読み取り
			ReadToken(linestream,ELEMENTNAME_CONTROLINDEX,readu);
			//読み取った値がuと異なる場合は例外を投げる
			if(readu!=u)
			{
				std::string msg("invalid file format (invalid Control Index) : ");
				msg+=ConfigFilePath;
				throw std::ios_base::failure(msg);
			}
			//LMotorSpeedの読み取り
			ReadToken(linestream,ELEMENTNAME_CONTROL_LMOTORSPEED,Control[i][u].LMotorSpeed);
			//RMotorSpeedの読み取り
			ReadToken(linestream,ELEMENTNAME_CONTROL_RMOTORSPEED,Control[i][u].RMotorSpeed);

			//StateCountの行に余計な情報が入っている場合は例外を投げる
			this->CheckUnnecessaryData(linestream);
		}//next u
	}//next i

	//Control[][]正当性チェック
	for(idx i=0;i<StateCount;i++)
	{
		//ControlCount[i]の取得
		const idx controlcount=ControlCount[i];

		for(idx u=0;u<controlcount;u++)
		{
			//Control[i][u]の値のチェック
			//MotorSpeedが[-SPEEDMAX,SPEEDMAX]の範囲に入っていなければ例外を投げる
			//LMotorSpeedのチェック
			if((Control[i][u].LMotorSpeed<-SPEEDMAX)||(Control[i][u].LMotorSpeed>SPEEDMAX))
			{
				std::string msg("invalid data ("+ELEMENTNAME_CONTROL_LMOTORSPEED+") : ");
				msg+=ConfigFilePath;
				throw std::ios_base::failure(msg);
			}
			//LMotorSpeedのチェック
			if((Control[i][u].RMotorSpeed<-SPEEDMAX)||(Control[i][u].RMotorSpeed>SPEEDMAX))
			{
				std::string msg("invalid data ("+ELEMENTNAME_CONTROL_RMOTORSPEED+") : ");
				msg+=ConfigFilePath;
				throw std::ios_base::failure(msg);
			}
		}//next u
	}//next i
}
//設定ファイル読み込み RegularPolicy取得
void EV3LineTracer::ReadRegularPolicy(std::ifstream& fin)
{
	RegularPolicy=Policy(StateCount);
	//StateCount回繰り替えしStateごとに読み込む
	for(idx i=0;i<StateCount;i++)
	{
		//設定ファイルから読み取った1行に対応するstring stream
		istringstream linestream;
		//設定ファイルからの1行読み取り
		ReadLine(fin,linestream);

		//読み取ったstateのindex
		idx readi;
		//state indexの読み取り
		ReadToken(linestream,ELEMENTNAME_STATEINDEX,readi);
		//読み取った値がiと異なる場合は例外を投げる
		if(readi!=i)
		{
			std::string msg("invalid file format (invalid State Index) : ");
			msg+=ConfigFilePath;
			throw std::ios_base::failure(msg);
		}
		//control読み取り用
		idx readu;
		//state i に対するcontrolの読み取り
		ReadToken(linestream,ELEMENTNAME_REGULARPOLICY,readu);
		RegularPolicy.SetPolicy(i,readu);
		if(RegularPolicy[i]>=ControlCount[i])
		{
			std::string msg("invalid file format (invalid Regular Policy) : ");
			msg+=ConfigFilePath;
			throw std::ios_base::failure(msg);
		}
		//StateCountの行に余計な情報が入っている場合は例外を投げる
		this->CheckUnnecessaryData(linestream);
	}//next i
}

//設定ファイル読み込みを行う
void EV3LineTracer::ReadConfigFile()
{
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


