/*
 * TimeToString.h
 *
 *  Created on: 2015/09/06
 *      Author: daisuke
 */

#ifndef TIMETOSTRING_H_
#define TIMETOSTRING_H_

#include <ctime>
#include <string>

namespace RL
{

class TimeToString
{
public:
	static std::string toString(const std::time_t& time_)
	{
		const int size = 64;
		char timestring[size];
		std::tm* construct_time_tm = std::localtime(&time_);
		char pattern[] = "%Y/%m/%d %H:%M:%S";
		std::strftime(timestring,size,pattern,construct_time_tm);

		return std::string(timestring);
	}
};

} /* namespace RL */

#endif /* TIMETOSTRING_H_ */
