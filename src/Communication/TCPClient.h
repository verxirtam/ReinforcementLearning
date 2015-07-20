/*
 * TCPClient.h
 *
 *  Created on: 2015/07/20
 *      Author: daisuke
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include <sys/types.h>
#include <iostream>
#include <string>

#include "TCPConnection.h"
#include "TCPStreamBuf.h"
#include "TSVInputContext.h"
#include "TSVOutputContext.h"

namespace RL
{

class TCPClient
{
private:
	TCPConnection tcpConnection;
	TCPStreamBuf tcpStreamBuf;
	std::iostream ioTcpStream;
	TSVInputContext tsvInputContext;
	TSVOutputContext tsvOutputContext;
public:
	TCPClient(std::string server_adress, int port, uint beffer_size);
	virtual ~TCPClient();
	InputContext& getInputContext()
	{
		return tsvInputContext;
	}
	OutputContext& getOutputContext()
	{
		return tsvOutputContext;
	}
};

} /* namespace RL */

#endif /* TCPCLIENT_H_ */
