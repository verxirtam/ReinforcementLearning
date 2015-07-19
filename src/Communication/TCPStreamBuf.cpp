/*
 * TCPStreamBuf.cpp
 *
 *  Created on: 2015/07/20
 *      Author: daisuke
 */

#include "TCPStreamBuf.h"


namespace RL
{

TCPStreamBuf::int_type TCPStreamBuf::underflow()
{
	//読み取った文字数(>0)またはEOF(=0)
	ssize_t read_result;

	//bufferSize文字分読み込みバッファに読み込む
	//エラーの場合はこの関数内で例外発生
	read_result = tcpConnection.read(readBuffer, this->bufferSize);

	if(read_result == 0)
	{
		//readの結果が0の時はEOFに達したことを通知
		return traits_type::eof();
	}

	//読み込めた文字数に応じて読み込みバッファを設定
	this->setg(this->eback(),this->eback(),this->eback()+read_result);
	//読み込んた最初の文字の値をint_typeで返却
	return traits_type::to_int_type(*this->gptr());
}

TCPStreamBuf::int_type TCPStreamBuf::overflow(int_type __c)
{
	//書き込みバッファの内容を送信する
	//エラーの場合はこの関数内で例外発生
	tcpConnection.write(this->pbase(), this->pptr() - this->pbase());//this->length);
	//書き込みバッファの設定を初期化する
	setp(writeBuffer, writeBuffer + this->bufferSize);

	//入力された文字を書き込む
	this->sputc(__c);

	return traits_type::to_int_type(__c);

}
int TCPStreamBuf::sync()
{
	//書き込みバッファの内容を書き出す
	tcpConnection.write(this->pbase(), this->pptr() - this->pbase());

	return 0;
}

TCPStreamBuf::TCPStreamBuf(TCPConnection& tcpconnection, uint buffersize):
		bufferSize(buffersize),
		tcpConnection(tcpconnection)
{
	readBuffer = NULL;
	writeBuffer = NULL;
	try
	{
		readBuffer = new char[bufferSize];
		writeBuffer = new char[bufferSize];
	}
	catch(...)
	{
		delete[] readBuffer;
		delete[] writeBuffer;
		throw;
	}
}

TCPStreamBuf::~TCPStreamBuf()
{
	delete[] readBuffer;
	delete[] writeBuffer;
}

} /* namespace RL */
