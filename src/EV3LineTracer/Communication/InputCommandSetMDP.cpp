/*
 * InputCommandSetMDP.cpp
 *
 *  Created on: 2015/08/18
 *      Author: daisuke
 */

#include "InputCommandSetMDP.h"



namespace RL
{

void InputCommandSetMDP::processOnNG(InputContext& input)
{
	std::ostringstream msg;
	msg << "SetMDP NG" << std::endl;
	while (true)
	{
		if (!input.hasNextToken())
		{
			input.skipReturn();
			msg << std::endl;
		}
		std::string token = input.nextToken();
		if (token == "")
		{
			break; //while
		}
		msg << token;
		msg << '\t';
	}
	throw std::ios_base::failure(msg.str());
}
bool InputCommandSetMDP::equalsOfEV3LineTracer(const EV3LineTracer& ev3)const
{
	if(ev3LineTracer.getInterval()  !=ev3.getInterval()){return false;}
	if(ev3LineTracer.getCostMax()   !=ev3.getCostMax()){return false;}
	if(ev3LineTracer.getStateCount()!=ev3.getStateCount()){return false;}
	idx state_count=ev3LineTracer.getStateCount();
	for(idx i = 0; i < state_count; i++)
	{
		real diff_of_refmax = ev3LineTracer.getState(i).refMax-ev3.getState(i).refMax;
		bool state_equals = (std::fabs(diff_of_refmax) < 0.125*0.125*0.125 );
		if(!state_equals){return false;}
		if(ev3LineTracer.getControlCount(i)!=ev3.getControlCount(i)){return false;}
		idx control_count =ev3LineTracer.getControlCount(i);
		for(idx u = 0; u < control_count; u++)
		{
			bool control_equals_l=(ev3LineTracer.getControl(i,u).lMotorSpeed==ev3.getControl(i,u).lMotorSpeed);
			bool control_equals_r=(ev3LineTracer.getControl(i,u).rMotorSpeed==ev3.getControl(i,u).rMotorSpeed);
			bool control_equals =(control_equals_l==control_equals_r);
			if(!control_equals){return false;}
		}
	}
	Policy regular_policy0;
	Policy regular_policy1;
	ev3LineTracer.getRegularPolicy(regular_policy0);
	ev3.getRegularPolicy(regular_policy1);
	if(regular_policy0!=regular_policy1){return false;}
	return true;
}

void InputCommandSetMDP::process(InputContext &input)
{
	input.skipToken("SetMDP");
	input.skipReturn();
	std::string result = input.nextToken();
	input.skipReturn();
	if(result == "OK")
	{
		EV3LineTracer ev3;
		RL::ReadInterval(ev3).process(input);
		RL::ReadCostMax(ev3).process(input);
		RL::ReadStateCount(ev3).process(input);
		RL::ReadState(ev3).process(input);
		RL::ReadControl(ev3).process(input);
		RL::ReadRegularPolicy(ev3).process(input);
		if(!equalsOfEV3LineTracer(ev3))
		{
			throw std::ios_base::failure("SetMDPに失敗しました");
		}
		return;
	}
	processOnNG(input);

}
} /* namespace RL */
