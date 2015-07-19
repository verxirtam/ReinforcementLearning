/*
 * TCPConnection.h
 *
 *  Created on: 2015/07/20
 *      Author: daisuke
 */

#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include <netinet/in.h>
#include <stddef.h>
#include <sys/socket.h>
#include <cstring>
#include <string>
#include <unistd.h>   // fork, read, write, open, close
#include <netdb.h>   // ソケット（hostent等）
#include <iostream>




namespace RL
{

class TCPConnection
{
private:
	//サーバ接続用のクライアント側ソケット
	int ClientSocket;
	//接続先サーバのポート番号
	int Port;
	//サーバ名
	std::string ServerName;

	//サーバとの接続を確立する
	void init();
public:
	TCPConnection(std::string serveraddress,int port):
		ClientSocket(-1),
		Port(port),
		ServerName(serveraddress)
	{
		this->init();
	}
	virtual ~TCPConnection();
	//受信結果の読み取り
	//Read NBYTES into BUF from FD.
	//Return the number read, -1 for errors or 0 for EOF.
	ssize_t read(char* buffer, size_t size);
	//接続先へのデータの送信
	ssize_t write(const char* buffer, size_t size);

};

} /* namespace RL */

#endif /* TCPCONNECTION_H_ */
