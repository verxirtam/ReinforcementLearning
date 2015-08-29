/*
 * WriteValue.h
 *
 *  Created on: 2015/08/29
 *      Author: daisuke
 */

#ifndef WRITEVALUE_H_
#define WRITEVALUE_H_

#include "../OutputContext.h"
#include "../OutputProcedure.h"

namespace RL
{

template<class MDP, typename ValueType>
class WriteValue: public OutputProcedure
{
private:
	//書き込みを行いたい変数のゲッターを示す関数ポインタ
	typedef ValueType (MDP::*GetterType)()const;
	//セッターを持つMDP
	MDP& mdp;
	//ゲッター
	GetterType getter;
public:
	WriteValue(MDP& mdp_, GetterType getter_) :
			mdp(mdp_), getter(getter_)
	{
	}
	virtual ~WriteValue()
	{
	}
	virtual void process(OutputContext& output)
	{
		ValueType v = (mdp.*getter)();
		output.writeToken(std::to_string(v));
		output.newLine();
	}
};

} /* namespace RL */

#endif /* WRITEVALUE_H_ */
