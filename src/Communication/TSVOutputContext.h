/*
 * TSVOutputContext.h
 *
 *  Created on: 2014/12/14
 *      Author: daisuke
 */

#ifndef TSVOUTPUTCONTEXT_H_
#define TSVOUTPUTCONTEXT_H_

#include <ostream>
#include "OutputContext.h"

namespace RL
{

class TSVOutputContext: public OutputContext
{
private:
	bool atLineStart;
	std::ostream &output;
public:
	TSVOutputContext(std::ostream &os):atLineStart(true),output(os)
	{
	}
	virtual ~TSVOutputContext(){}
	virtual void writeToken(std::string s);
	virtual void newLine(void);
	//書き込みバッファの内容を反映する
	virtual void flush()
	{
		output.flush();
	}
};

} /* namespace RL */

#endif /* TSVOUTPUTCONTEXT_H_ */
