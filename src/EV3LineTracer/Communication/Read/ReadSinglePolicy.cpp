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

ReadSinglePolicy::ReadSinglePolicy(EV3LineTracer& ev3, Policy& p, idx i) :
		ev3LineTracer(ev3),
		policy(p),
		stateIndex(0)
{
	setStateIndex(i);

}

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
	idx controlcount = ev3LineTracer.GetControlCount(i);
	idx u = StringToData::parseTo<idx>(input.nextToken());
	if((u < 0) || (u >= controlcount) )
	{
		std::string msg("invalid controlIndex");
		throw std::ios_base::failure(msg);
	}
	input.skipReturn();

	policy.SetPolicy(i,u);
}

} /* namespace RL */
