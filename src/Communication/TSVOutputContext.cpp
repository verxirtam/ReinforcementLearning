/*
 * TSVOutputContext.cpp
 *
 *  Created on: 2014/12/14
 *      Author: daisuke
 */

#include "TSVOutputContext.h"

namespace RL
{

using namespace std;

void TSVOutputContext::writeToken(std::string s)
{
	if(atLineStart)
	{
		atLineStart=false;
	}
	else
	{
		output<<"\t";
	}
	output<<s;
}

void TSVOutputContext::newLine(void)
{
	output<<endl;
	atLineStart=true;
}

} /* namespace RL */
