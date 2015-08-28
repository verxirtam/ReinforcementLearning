/*
 * ReadValue.h
 *
 *  Created on: 2015/08/29
 *      Author: daisuke
 */

#ifndef READVALUE_H_
#define READVALUE_H_

#include "../../StringToData.h"
#include "../InputContext.h"
#include "../InputProcedure.h"

namespace RL
{

template<class MDP, typename ValueType, typename ReturnType = void>
class ReadValue: public InputProcedure
{
private:
	//読み取りを行いたい変数のセッターを示す関数ポインタ
	typedef ReturnType (MDP::*SetterType)(ValueType);
	//セッターを持つMDP
	MDP& mdp;
	//セッター
	SetterType setter;
public:
	ReadValue(MDP& mdp_, SetterType setter_):mdp(mdp_),setter(setter_)
	{
	}
	virtual ~ReadValue()
	{
	}
	virtual void process(InputContext &input)
	{
		//読み取り用変数
		ValueType v;
		//トークンをValueType型として読み取り
		v = StringToData::parseTo<ValueType>(input.nextToken());
		//改行を読み飛ばす
		input.skipReturn();
		//mdpに対してsetterでvを設定
		(mdp.*setter)(v);
	}
};

} /* namespace RL */

#endif /* READVALUE_H_ */
