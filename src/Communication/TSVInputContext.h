/*
 * TSVInputContext.h
 *
 *  Created on: 2014/12/14
 *      Author: daisuke
 */

#ifndef TSVINPUTCONTEXT_H_
#define TSVINPUTCONTEXT_H_

#include <istream>
#include "InputContext.h"

namespace RL
{

class TSVInputContext: public InputContext
{
private:
	std::istream &input;
	//std::istream &line;
public:
	TSVInputContext(std::istream &is):input(is){}
	virtual ~TSVInputContext(){}

	// 次のTokenを取得し現在のTokenを1つ進める
	virtual std::string nextToken();

	// 次のTokenがskipStringであることを確認し現在のTokenを1つ進める
	virtual void skipToken(std::string skipString);

	// 次のTokenが行末であることを確認し現在のTokenを次の行の初めに進める
	virtual void skipReturn(void);

	// 現在の行で次のTokenがあるかどうかを確認する
	virtual bool hasNextToken(void);
};

} /* namespace RL */

#endif /* TSVINPUTCONTEXT_H_ */
