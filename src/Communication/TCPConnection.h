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

	//代入演算子(使用禁止)
	TCPConnection& operator=(const TCPConnection&);
	//コピーコンストラクタ(使用禁止)
	TCPConnection(const TCPConnection&);


public:
	TCPConnection():
		ClientSocket(-1),
		Port(-1),
		ServerName("")
	{
	}
	TCPConnection(std::string serveraddress,int port):
		ClientSocket(-1),
		Port(port),
		ServerName(serveraddress)
	{
		this->init();
	}
	void connect(std::string serveraddress,int port)
	{
		//接続済みのソケットのクローズ
		this->close();

		//ポート、サーバ名の設定
		Port = port;
		ServerName = serveraddress;

		//新規の接続設定
		this->init();
	}
	void close()
	{
		if(ClientSocket >= 0)
		{
			//接続を終了する。
			::close(ClientSocket);
			ClientSocket = -1;

		}
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
