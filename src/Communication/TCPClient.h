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
	//TCP接続
	TCPConnection tcpConnection;
	//TCP送受信データのバッファ
	TCPStreamBuf tcpStreamBuf;
	//TCP接続の入出力
	std::iostream ioTcpStream;
	//TCP接続のTSVとしての入力
	TSVInputContext tsvInputContext;
	//TCP接続のTSVとしての出力
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
