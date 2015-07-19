/*
 * TCPClient.h
 *
 *  Created on: 2015/07/20
 *      Author: daisuke
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include <iostream>

#include "TCPConnection.h"
#include "TCPStreamBuf.h"

namespace RL
{

class TCPClient
{
private:
	TCPConnection tcpConnection;
	TCPStreamBuf tcpStreamBuf;
	std::iostream ioTcpStream;
public:
	TCPClient(std::string server_adress, int port, uint beffer_size);
	virtual ~TCPClient();
	const std::iostream& getIOTcpStream() const
	{
		return ioTcpStream;
	}
};

} /* namespace RL */

#endif /* TCPCLIENT_H_ */
