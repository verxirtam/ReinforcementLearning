/*
 * TCPStreamBuf.h
 *
 *  Created on: 2015/07/20
 *      Author: daisuke
 */

#ifndef TCPSTREAMBUF_H_
#define TCPSTREAMBUF_H_

#include <sys/types.h>
#include <streambuf>
#include <string>

#include "TCPConnection.h"

namespace RL
{

class TCPStreamBuf: public std::streambuf
{
private:
	uint bufferSize;
	//読み込み用文字列バッファ
	char* readBuffer;
	//書き込み用文字列バッファ
	char* writeBuffer;
	//対応するコネクション
	TCPConnection& tcpConnection;
	//コピーコンストラクタ
	//コネクションはコピーできないため使用禁止
	TCPStreamBuf(const TCPStreamBuf&);
	//代入演算子
	//コネクションはコピーできないため使用禁止
	TCPStreamBuf& operator=(const TCPStreamBuf&);
protected:
	virtual int_type underflow();
	virtual int_type overflow(int_type __c = traits_type::eof());
	virtual int sync();
public:
	TCPStreamBuf(TCPConnection& tcpconnection, uint buffersize = 1024);
	virtual ~TCPStreamBuf();
	void setBufferSize(uint buffersize);
};

} /* namespace RL */

#endif /* TCPSTREAMBUF_H_ */
