/*
 * TCPConnection.cpp
 *
 *  Created on: 2015/07/20
 *      Author: daisuke
 */

#include "TCPConnection.h"



namespace RL
{


//サーバとの接続を確立する
void TCPConnection::init()
{
	hostent *server_inf1;   // クライアント情報１
	sockaddr_in server_inf2;   // クライアント情報２

	//サーバ名からサーバ情報を得る
	//gethostbyname()の結果はfreeの必要は無い
	//(同一の領域がgethostbyname()の実行の度に使用されるため)
	if ((server_inf1 = gethostbyname(ServerName.c_str())) == NULL)
	{
		//名前解決に失敗した場合
		std::string msg("***error***  サーバ名が不適当です\n");
		throw std::ios_base::failure(msg);
	}
	else
	{
		//サーバ情報をsockaddr_in構造体に設定
		memset((char *)&server_inf2, 0, sizeof(server_inf2));
		server_inf2.sin_family		= AF_INET;		//アドレスファミリー
		server_inf2.sin_port		= htons(Port);	//htons（２バイト）
													//整数の上位及び下位バイトの指定

		memcpy((char *)&server_inf2.sin_addr.s_addr, server_inf1->h_addr,
				server_inf1->h_length);

		//ソケットを作成
		if ((ClientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			//失敗した場合
			std::string msg("***error***  socketを作成できませんでした\n");
			throw std::ios_base::failure(msg);
		}
		else
		{
			//サーバへ接続
			if (::connect(ClientSocket, (sockaddr *)&server_inf2, sizeof(server_inf2)) == -1)
			{
				ClientSocket = -1;
				std::string msg("***error***  サーバへの接続に失敗しました\n");
				throw std::ios_base::failure(msg);
			}
		}
	}
}




TCPConnection::~TCPConnection()
{
	this->close();
}

//受信結果の読み取り
//Read NBYTES into BUF from FD.
//Return the number read, -1 for errors or 0 for EOF.
ssize_t TCPConnection::read(char* buffer, size_t size)
{
	ssize_t result = ::read(ClientSocket,buffer,size);

	if(result == -1)
	{
		//readの結果が-1の時はエラーのため例外発生
		std::string msg("メッセージ受信処理でエラーが発生しました");
		throw std::ios_base::failure(msg);
	}

	return result;
}
//接続先へのデータの送信
//書き込んだ文字数を返却
ssize_t TCPConnection::write(const char* buffer, size_t size)
{
	ssize_t result = ::write(ClientSocket,buffer,size);

	if(result == -1)
	{
		//readの結果が-1の時はエラーのため例外発生
		std::string msg("メッセージ送信処理でエラーが発生しました");
		throw std::ios_base::failure(msg);
	}
	return result;
}


} /* namespace RL */
