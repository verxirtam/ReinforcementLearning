/*
 * OutputContext.h
 *
 *  Created on: 2014/12/14
 *      Author: daisuke
 */

#ifndef OUTPUTCONTEXT_H_
#define OUTPUTCONTEXT_H_

#include <string>

namespace RL
{

class OutputContext
{
public:
	virtual ~OutputContext(){}
	//トークンを書き込む
	virtual void writeToken(std::string s)=0;
	//次の行に映る
	virtual void newLine(void)=0;
	//書き込みバッファの内容を反映する
	virtual void flush()=0;
};

} /* namespace RL */

#endif /* OUTPUTCONTEXT_H_ */
