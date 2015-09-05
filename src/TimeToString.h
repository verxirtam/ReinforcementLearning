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
private:
public:
	//time_t型の時刻を文字列に変換する("YYYY/MM/DD HH:MM:SS")
	static std::string toString(const std::time_t& time_)
	{
		std::string pattern("%Y/%m/%d %H:%M:%S");

		return TimeToString::toStringPattern(time_,pattern);
	}
	//time_t型の時刻を文字列に変換する("YYYYMMDDHHMMSS")
	static std::string toStringForFileName(const std::time_t& time_)
	{
		std::string pattern("%Y%m%d%H%M%S");

		return TimeToString::toStringPattern(time_,pattern);
	}
	//time_t型の時刻をpatternに応じて文字列に変換する
	static std::string toStringPattern(const std::time_t& time_, const std::string& pattern,const int size = 64)
	{
		char timestring[size];
		std::tm* construct_time_tm = std::localtime(&time_);
		std::strftime(timestring,size,pattern.c_str(),construct_time_tm);

		return std::string(timestring);
	}

};

} /* namespace RL */

#endif /* TIMETOSTRING_H_ */
