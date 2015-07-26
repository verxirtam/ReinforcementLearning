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

	//最初に読み込んだ文字へのポインタ
	char* gptr=this->gptr();
	//読み込んた最初の文字の値をint_typeで返却
	return traits_type::to_int_type(*gptr);
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
	//書き込みバッファの設定を初期化する
	setp(writeBuffer, writeBuffer + this->bufferSize);

	return 0;
}

TCPStreamBuf::TCPStreamBuf(TCPConnection& tcpconnection, uint buffersize):
		bufferSize(buffersize),
		readBuffer(NULL),
		writeBuffer(NULL),
		tcpConnection(tcpconnection)
{
	try
	{
		readBuffer = new char[bufferSize];
		writeBuffer = new char[bufferSize];

		//読み込み対象の文字列を設定する
		//ここでは即座にアンダーフローさせるために全て同じアドレスを指定する
		setg(readBuffer, readBuffer, readBuffer);
		//書き込み対象の文字列を設定する
		setp(writeBuffer, writeBuffer + bufferSize);
	}
	catch(...)
	{
		delete[] readBuffer;
		readBuffer=NULL;
		delete[] writeBuffer;
		writeBuffer=NULL;
		throw;
	}
}

TCPStreamBuf::~TCPStreamBuf()
{
	delete[] readBuffer;
	readBuffer=NULL;
	delete[] writeBuffer;
	writeBuffer=NULL;
}

void TCPStreamBuf::setBufferSize(uint buffersize)
{
	//バッファのサイズを小さくするような変更は無視する
	if(buffersize <= this->bufferSize)
	{
		return;
	}
	//新しいバッファサイズの反映
	this -> bufferSize = buffersize;

	//新たに確保するバッファのポインタ
	char* new_read_buffer = NULL;
	char* new_write_buffer = NULL;

	try
	{
		//書き込みバッファの内容を同期する
		this->sync();

		//新しい読み込みバッファの確保
		new_read_buffer = new char[bufferSize];
		//バッファに読み込み済みの文字数を取得
		int read_current = this->gptr() - this->eback();
		//バッファに読み込み済みの文字数を取得
		int read_size = this->egptr() - this->eback();

		//readBufferの内容をコピーする
		for(int i=0;i<read_size;i++)
		{
			new_read_buffer[i] = readBuffer[i];
		}

		//古い読み込みバッファの開放
		delete[] readBuffer;readBuffer=NULL;
		//新しい書き込みバッファへのポインタを設定する
		readBuffer = new_read_buffer;
		//読み込み対象の文字列を設定する
		setg(readBuffer, readBuffer+read_current, readBuffer+read_size);

		//新しい書き込みバッファの確保
		new_write_buffer = new char[bufferSize];
		//古い書き込みバッファの開放
		delete[] writeBuffer;writeBuffer=NULL;
		//新しい書き込みバッファへのポインタを設定する
		writeBuffer = new_write_buffer;
		//書き込み対象の文字列を設定する
		//sync()済みなのでバッファのコピーは不要
		setp(writeBuffer, writeBuffer + bufferSize);
	}
	catch(...)
	{
		delete[] new_read_buffer;
		new_read_buffer=NULL;
		delete[] new_write_buffer;
		new_write_buffer=NULL;
		throw;
	}
}


} /* namespace RL */
