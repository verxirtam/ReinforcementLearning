/*
 * EV3LineTracer.h
 *
 *  Created on: 2014/01/21
 *      Author: daisuke
 */

#ifndef EV3LINETRACER_H_
#define EV3LINETRACER_H_

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "../Communication/TCPClient.h"
#include "../RLUtility.h"
#include "../Policy.h"



namespace RL
{



struct EV3LineTracerState
{
	//黒線検知用の光センサの反射率(0.0～1.0)
	real RefMax;
};

typedef int speed;

struct EV3LineTracerControl
{
	//左モータのスピード(degrees/sec)
	speed LMotorSpeed;
	//右モータのスピード(degrees/sec)
	speed RMotorSpeed;
};


class EV3LineTracer
{
	//フレンドクラス
	//InputProsedureクラスの派生クラスで
	//EV3LineTracerクラスにアクセスするものを宣言
	friend class ReadInterval;
	friend class ReadStateCount;
	friend class ReadSingleState;
	friend class ReadSingleControl;
	friend class ReadRegularPolicy;

private:
	//データメンバ
	/////////////////////////////
	//Controlを維持する間隔(msec)
	idx Interval;
	//状態数
	idx StateCount;
	//状態
	std::vector<EV3LineTracerState> State;
	//Control数
	std::vector<idx> ControlCount;
	//Control
	std::vector<std::vector<EV3LineTracerControl> > Control;
	//現在設定されているPolicy;
	StochasticPolicy CurrentPolicy;
	//RegularPolicy コストの基準となるポリシー
	Policy RegularPolicy;
	//コストの最大値(RegularPolicyの10倍に定める)
	real CostMax;
	//設定ファイルのパス
	std::string ConfigFilePath;
	//モータに与える速度の最大値
	static const speed SPEEDMAX=600;
	//TCP接続のクライアント
	TCPClient tcpClient;
	//メンバ関数
	/////////////////////////////

	//Intervalの設定
	void setInterval(idx i)
	{
		Interval=i;
	}
	//StateCountの設定
	void setStateCount(idx sc)
	{
		//StateCountの設定
		StateCount=sc;
		//State[]のサイズを変更
		State.resize(StateCount);
		//ControlCount[]のサイズを変更
		ControlCount.resize(StateCount);
		//Control[]のサイズを変更
		Control.resize(StateCount);

	}
	//State[stateindex]の設定
	void setState(idx stateindex,real refmax,idx controlcount)
	{
		//stateindexのチェック
		checkStateIndex(stateindex);
		//refmaxのチェック
		if(refmax<0.0 || 1.0 < refmax)
		{
			std::stringstream msg;
			msg<<"refmax is valid: refmax = " << refmax << ".";

			throw std::ios_base::failure(msg.str());

		}
		//State[stateindex]の設定
		State[stateindex].RefMax = refmax;
		//ControlCountの設定
		ControlCount[stateindex] = controlcount;
		//Control[stateindex]のサイズを変更
		Control[stateindex].resize(controlcount);

	}
	void setControl(idx stateindex,idx controlindex,idx lmoterspeed,idx rmoterspeed)
	{
		//stateindex, controlindexのチェック
		checkIndex(stateindex,controlindex);

		Control[stateindex][controlindex].LMotorSpeed = lmoterspeed;
		Control[stateindex][controlindex].RMotorSpeed = rmoterspeed;

	}
	void setRegularPolicy(const Policy& p)
	{
		RegularPolicy=p;
	}

	void checkStateIndex(idx stateindex)
	{
		//stateindexのチェック
		if (stateindex < 0 || this->StateCount <= stateindex)
		{
			std::stringstream msg;
			msg << "stateindex is valid: stateindex = " << stateindex << ".";
			throw std::ios_base::failure(msg.str());
		}
	}
	void checkIndex(idx stateindex, idx controlindex)
	{
		//stateindexのチェック
		checkStateIndex(stateindex);
		//controlindexのチェック
		if (controlindex < 0 || this->ControlCount[stateindex] <= controlindex)
		{
			std::stringstream msg;
			msg << "controlindex is valid: controlindex = " << controlindex << ".";
			throw std::ios_base::failure(msg.str());
		}
	}
public:
	//デフォルトコンストラクタ
	EV3LineTracer():
		Interval(0),
		StateCount(0),
		ControlCount(),
		Control(),
		CurrentPolicy(),
		CostMax(0.0),
		ConfigFilePath(),
		tcpClient("localhost",50000,1024)
	{
	}
	//コンストラクタ
	EV3LineTracer(std::string configfilepath):
		Interval(0),
		StateCount(0),
		ControlCount(),
		Control(),
		CurrentPolicy(),
		CostMax(0.0),
		ConfigFilePath(configfilepath),
		tcpClient("localhost",50000,1024)
	{
	}
	//コピーコンストラクタ→動的確保するメンバ変数は無いので不要
	//コピー代入演算子→動的確保するメンバ変数は無いので不要
	//設定ファイル読み込みを行う
	void ReadConfigFile();

	//EV3の準備を行う
	void InitEV3();

	//設定ファイル読み込みを行い、パラメータを初期化する
	void Init();

	inline idx GetInterval()const
	{
		return Interval;
	}

	inline idx GetStateCount()const
	{
		return StateCount;
	}
	inline EV3LineTracerState GetState(idx i)const
	{
		return State[i];
	}
	inline EV3LineTracerControl getControl(idx i,idx u)const
	{
		return Control[i][u];
	}

	inline idx GetControlCount(idx i)const
	{
		return ControlCount[i];
	}
	//MDPに適合した標準的なポリシーの取得
	Policy& GetRegularPolicy(Policy& out)const
	{
		out=RegularPolicy;
		return out;
	}
	//TODO 下記の関数を作成する
	//Mdp.GetEpisode(e)
	//割引率の取得(1.0を返すのみ)
	inline real GetDiscountRate()const
	{
		return 1.0;
	}
	//コストの最大値を取得
	inline real GetCostMax()const
	{
		return CostMax;
	}
	//PolicyからStochasticPolicyを得る
	//TODO SimpleMDPと内容が同一の関数。共通化する方法を検討する
	//案：Policyのメンバ関数(MDPはテンプレートクラス)として作成する：
	//		template<typename MDP>
	//		StochasticPolicy& Policy::GetStochasticPolicy(MDP& mdp,StochasticPolicy& out)const
	StochasticPolicy& GetStochasticPolicy(const Policy& p,StochasticPolicy& out)const
	{
		p.Verify(*this);
		StochasticPolicyData data(p.GetStateCount());
		for(idx i=0;i<data.size();i++)
		{
			std::vector<real> prob(this->GetControlCount(i),0.0);
			prob[p[i]]=1.0;
			data[i]=RandomIdx(prob,16);
		}
		out=StochasticPolicy(data);

		return out;
	}
	//CurrentPolicyの設定
	//TODO SimpleMDPと内容が同一の関数。共通化する方法を検討する
	inline void SetCurrentPolicy(const Policy& p)
	{
		StochasticPolicy sp;
		this->SetCurrentPolicy(this->GetStochasticPolicy(p,sp));
	}
	//CurrentPolicyの設定
	//TODO SimpleMDPと内容が同一の関数。共通化する方法を検討する
	inline void SetCurrentPolicy(const StochasticPolicy& p)
	{
		p.Verify(*this);
		this->CurrentPolicy=p;
	}
	inline real getMotorSpeedMax(void) const
	{
		return SPEEDMAX;
	}
	inline real getMotorSpeedMin(void) const
	{
		return -SPEEDMAX;
	}
	//何もしないコマンドを発行する
	//通信の確率の確認に使用する
	void execNullCommand()
	{
		throw std::ios::failure("未実装");
		//TODO Init()にEV3へのコネクションを貼るコードを書く
		//TODO コネクションはデータメンバとして保持する
		//TODO EV3への送信用のデータ(NULLコマンド)を作成
		//TODO EV3へ送信
		//TODO EV3からの返信を受信する
		//TODO 結果を読み取る
		//TODO 結果に応じた処理を実行する
	}
};



}//namespace RL
#endif /* EV3LINETRACER_H_ */
