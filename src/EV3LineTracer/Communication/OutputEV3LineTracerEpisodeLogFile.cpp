/*
 * OutputEV3LineTracerEpisodeLogFile.cpp
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#include "OutputEV3LineTracerEpisodeLogFile.h"


namespace RL
{
void OutputEV3LineTracerEpisodeLogFile::process(OutputContext& output)
{
	double elapsed_time =std::difftime(finishTime, startTime);

	output.writeToken("StartTime");
	output.writeToken(TimeToString::toString(startTime));
	output.newLine();
	output.writeToken("FinishTime");
	output.writeToken(TimeToString::toString(finishTime));
	output.newLine();
	output.writeToken("ElapsedTime");
	output.writeToken(std::to_string(elapsed_time));
	output.newLine();
	output.writeToken(ev3LineTracer.getSettingFilePath());
	output.newLine();
	WriteCurrentPolicy<EV3LineTracer>(ev3LineTracer).process(output);
	WriteEpisode(episode).process(output);
}
} /* namespace RL */
