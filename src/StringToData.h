/*
 * StringToData.h
 *
 *  Created on: 2014/12/21
 *      Author: daisuke
 */

#ifndef STRINGTODATA_H_
#define STRINGTODATA_H_

#include <string>
#include "RLUtility.h"

namespace RL
{

class StringToData
{
public:
	/*
	template<typename T>
	static void toData(const std::string &s, T &t);
	*/
	template<typename T>
	static T parseTo(const std::string &s);
};
/*
template<typename T>
void StringToData::toData(const std::string &s, T &t)
{
	std::istringstream iss(s);
	iss >> t;
}*/

template<typename T>
T StringToData::parseTo(const std::string &s)
{
	T t;
	std::istringstream iss(s);
	iss >> t;
	return t;
}


} /* namespace RL */

#endif /* STRINGTODATA_H_ */
