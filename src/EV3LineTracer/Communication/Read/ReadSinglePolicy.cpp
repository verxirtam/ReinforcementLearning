/*
 * ReadSinglePolicy.cpp
 *
 *  Created on: 2015/05/31
 *      Author: daisuke
 */

#include "ReadSinglePolicy.h"

#include <iostream>
#include <string>


namespace RL
{



ReadSinglePolicy::~ReadSinglePolicy()
{

}
void ReadSinglePolicy::process(InputContext &input)
{
	idx i = StringToData::parseTo<idx>(input.nextToken());
	if(i != stateIndex)
	{
		std::string msg("invalid stateIndex");
		throw std::ios_base::failure(msg);
	}
	idx u = StringToData::parseTo<idx>(input.nextToken());
	input.skipReturn();

	policy.setPolicy(i,u);
}

} /* namespace RL */
