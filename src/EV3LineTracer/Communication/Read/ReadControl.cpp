/*
 * ReadControl.cpp
 *
 *  Created on: 2015/05/31
 *      Author: daisuke
 */

#include "ReadControl.h"


namespace RL
{

ReadControl::ReadControl(EV3LineTracer &ev3) :
		ev3LineTracer(ev3)
{
}

ReadControl::~ReadControl()
{
	// TODO Auto-generated destructor stub
}
void ReadControl::process(InputContext &input)
{
	//state、control毎にReadSingleControl.process()を実行する

	ReadSingleControl rc(ev3LineTracer);

	idx statecount = ev3LineTracer.GetStateCount();
	for(idx i=0;i<statecount;i++)
	{
		rc.setStateIndex(i);

		idx controlcount = ev3LineTracer.GetControlCount(i);

		for(idx u=0; u < controlcount; u++)
		{
			rc.setControlIndex(u);

			//ReadSingleControlの実行
			rc.process(input);
		}
	}
}

} /* namespace RL */
