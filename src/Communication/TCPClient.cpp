/*
 * TCPClient.cpp
 *
 *  Created on: 2015/07/20
 *      Author: daisuke
 */

#include "TCPClient.h"

namespace RL
{

TCPClient::TCPClient(std::string server_adress, int port, uint beffer_size):
		tcpConnection(server_adress, port),
		tcpStreamBuf(tcpConnection,beffer_size),
		ioTcpStream(&tcpStreamBuf),
		tsvInputContext(ioTcpStream),
		tsvOutputContext(ioTcpStream)
{
}

TCPClient::~TCPClient()
{
}

} /* namespace RL */
