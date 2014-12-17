/*
 * InputContext.h
 *
 *  Created on: 2014/12/14
 *      Author: daisuke
 */

#ifndef INPUTCONTEXT_H_
#define INPUTCONTEXT_H_

#include <string>

namespace RL
{

class InputContext
{
public:
	virtual ~InputContext(){}

	// 次のTokenを取得し現在のTokenを1つ進める
	virtual std::string nextToken()=0;

	// 次のTokenがskipStringであることを確認し現在のTokenを1つ進める
	virtual void skipToken(std::string skipString)=0;

	// 次のTokenが行末であることを確認し現在のTokenを次の行の初めに進める
	virtual void skipReturn(void)=0;

	// 現在の行で次のTokenがあるかどうかを確認する
	virtual bool hasNextToken(void)=0;
};

} /* namespace RL */

#endif /* INPUTCONTEXT_H_ */
