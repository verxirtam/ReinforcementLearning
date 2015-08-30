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
#include <memory>

#include "../Communication/InputMessage_1_0.h"
#include "../Communication/OutputMessage_1_0.h"
#include "../Communication/TSVInputContext.h"
#include "../Communication/TCPClient.h"
#include "../RLUtility.h"
#include "../Policy.h"
#include "../Episode.h"

#include "Communication/InputEV3LineTracer_1_0.h"
#include "Communication/OutputEV3LineTracer_1_0.h"


namespace RL
{



struct EV3LineTracerState
{
	//黒線検知用の光センサの反射率(0.0～1.0)
	real refMax;
};

typedef int speed;

struct EV3LineTracerControl
{
	//左モータのスピード(degrees/sec)
	speed lMotorSpeed;
	//右モータのスピード(degrees/sec)
	speed rMotorSpeed;
};

//クラス内のフレンド宣言のための前方宣言
template <typename T>
class ReadRegularPolicy;

class EV3LineTracer
{
	//フレンドクラス
	//InputProsedureクラスの派生クラスで
	//EV3LineTracerクラスにアクセスするものを宣言
	friend class ReadInterval;
	friend class ReadStateCount;
	friend class ReadSingleState;
	friend class ReadSingleControl;
	friend class ReadRegularPolicy<EV3LineTracer>;
	friend class ReadCostMax;
	friend class InputConfigFileEV3Linetracer_1_0;

private:
	//データメンバ
	/////////////////////////////
	//Controlを維持する間隔(msec)
	idx interval;
	//状態数
	idx stateCount;
	//状態
	std::vector<EV3LineTracerState> state;
	//Control数
	std::vector<idx> controlCount;
	//Control
	std::vector<std::vector<EV3LineTracerControl> > control;
	//現在設定されているPolicy;
	StochasticPolicy currentPolicy;
	//RegularPolicy コストの基準となるポリシー
	Policy regularPolicy;
	//コストの最大値
	real costMax;
	//設定ファイルのパス
	std::string configFilePath;
	//モータに与える速度の最大値
	static const speed SPEEDMAX=600;
	//メンバ関数
	/////////////////////////////

	//Intervalの設定
	void setInterval(idx i)
	{
		interval=i;
	}
	//Intervalの設定
	void setCostMax(real c)
	{
		costMax = c;
	}

	//StateCountの設定
	void setStateCount(idx sc)
	{
		//StateCountの設定
		stateCount=sc;
		//State[]のサイズを変更
		state.resize(stateCount);
		//ControlCount[]のサイズを変更
		controlCount.resize(stateCount);
		//Control[]のサイズを変更
		control.resize(stateCount);

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
		state[stateindex].refMax = refmax;
		//ControlCountの設定
		controlCount[stateindex] = controlcount;
		//Control[stateindex]のサイズを変更
		control[stateindex].resize(controlcount);

	}
	void setControl(idx stateindex,idx controlindex,idx lmoterspeed,idx rmoterspeed)
	{
		//stateindex, controlindexのチェック
		checkIndex(stateindex,controlindex);

		control[stateindex][controlindex].lMotorSpeed = lmoterspeed;
		control[stateindex][controlindex].rMotorSpeed = rmoterspeed;

	}
	void setRegularPolicy(const Policy& p)
	{
		regularPolicy=p;
	}

	void checkStateIndex(idx stateindex)
	{
		//stateindexのチェック
		if (stateindex < 0 || this->stateCount <= stateindex)
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
		if (controlindex < 0 || this->controlCount[stateindex] <= controlindex)
		{
			std::stringstream msg;
			msg << "controlindex is valid: controlindex = " << controlindex << ".";
			throw std::ios_base::failure(msg.str());
		}
	}
	//コピーコンストラクタ→TCPConnectionのコピーを禁じるため禁止する
	EV3LineTracer(const EV3LineTracer &);
	//コピー代入演算子→TCPConnectionのコピーを禁じるため禁止する
	EV3LineTracer& operator=(const EV3LineTracer &);
	//コマンドの実行
	void execCommand(RL::OutputProcedure& o_command,RL::InputProcedure& i_command)const;
	//データメンバのcurrentPolicyを更新する
	//EV3側は変更しない
	void setCurrentPolicyLocal(const Policy& p)
	{
		StochasticPolicy sp;
		this->setCurrentPolicyLocal(this->getStochasticPolicy(p,sp));
	}
	//データメンバのcurrentPolicyを更新する
	//EV3側は変更しない
	void setCurrentPolicyLocal(const StochasticPolicy& p)
	{
		p.verify(*this);
		this->currentPolicy = p;
	}

public:
	//デフォルトコンストラクタ
	EV3LineTracer():
		interval(0),
		stateCount(0),
		state(),
		controlCount(),
		control(),
		currentPolicy(),
		regularPolicy(),
		costMax(0.0),
		configFilePath()
	{
	}
	//コンストラクタ
	EV3LineTracer(std::string configfilepath):
		interval(0),
		stateCount(0),
		state(),
		controlCount(),
		control(),
		currentPolicy(),
		regularPolicy(),
		costMax(0.0),
		configFilePath(configfilepath)
	{
	}
	//設定ファイル読み込みを行う
	void readConfigFile();

	//EV3の準備を行う
	void initEV3();

	//設定ファイル読み込みを行い、パラメータを初期化する
	void init();

	inline idx getInterval()const
	{
		return interval;
	}

	inline idx getStateCount()const
	{
		return stateCount;
	}
	inline EV3LineTracerState getState(idx i)const
	{
		return state[i];
	}
	inline EV3LineTracerControl getControl(idx i,idx u)const
	{
		return control[i][u];
	}

	inline idx getControlCount(idx i)const
	{
		return controlCount[i];
	}
	//MDPに適合した標準的なポリシーの取得
	Policy& getRegularPolicy(Policy& out)const
	{
		out=regularPolicy;
		return out;
	}
	//MDPに適合した標準的なポリシーの取得
	Policy getRegularPolicy()const
	{
		Policy out = regularPolicy;
		return out;
	}
	//MDPに適合した標準的なポリシーの取得
	StochasticPolicy& getCurrentPolicy(StochasticPolicy& out)const
	{
		out=this->currentPolicy;
		return out;
	}
	//MDPに適合した標準的なポリシーの取得
	StochasticPolicy getCurrentPolicy()const
	{
		StochasticPolicy out = this->currentPolicy;
		return out;
	}
	//TODO 下記の関数を作成する
	//Mdp.GetEpisode(e)
	//割引率の取得(1.0を返すのみ)
	inline real getDiscountRate()const
	{
		return 1.0;
	}
	//コストの最大値を取得
	inline real getCostMax()const
	{
		return costMax;
	}
	//PolicyからStochasticPolicyを得る
	//TODO SimpleMDPと内容が同一の関数。共通化する方法を検討する
	//案：Policyのメンバ関数(MDPはテンプレートクラス)として作成する：
	//		template<typename MDP>
	//		StochasticPolicy& Policy::GetStochasticPolicy(MDP& mdp,StochasticPolicy& out)const
	StochasticPolicy& getStochasticPolicy(const Policy& p,StochasticPolicy& out)const
	{
		p.verify(*this);
		StochasticPolicyData data(p.getStateCount());
		for(idx i=0;i<data.size();i++)
		{
			std::vector<real> prob(this->getControlCount(i),0.0);
			prob[p[i]]=1.0;
			data[i]=RandomIdx(prob,16);
		}
		out=StochasticPolicy(data);

		return out;
	}
	//CurrentPolicyの設定
	inline void setCurrentPolicy(const Policy& p)
	{
		//this->currentPolicyを設定
		StochasticPolicy sp;
		this->setCurrentPolicy(this->getStochasticPolicy(p,sp));
	}
	//CurrentPolicyの設定
	void setCurrentPolicy(const StochasticPolicy& p);

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
	void execNullCommand();

	//MDPの設定をEV3に反映する
	void execSetMDP();

	//エピソードを取得する
	bool getEpisode(Episode& episode)const;

};



}//namespace RL
#endif /* EV3LINETRACER_H_ */
