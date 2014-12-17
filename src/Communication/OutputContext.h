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
	virtual void writeToken(std::string s)=0;
	virtual void newLine(void)=0;
};

} /* namespace RL */

#endif /* OUTPUTCONTEXT_H_ */
