/*
 * TcpStreamBuf.h
 *
 *  Created on: 2015/07/19
 *      Author: daisuke
 */

#ifndef TCPSTREAMBUF_H_
#define TCPSTREAMBUF_H_

#include <streambuf>

namespace RL
{

class TcpStreamBuf: public std::streambuf
{
private:
	int bufferSize;
public:
	TcpStreamBuf();
	virtual ~TcpStreamBuf();
};

} /* namespace RL */

#endif /* TCPSTREAMBUF_H_ */
