/*
 * TSVInputContext.cpp
 *
 *  Created on: 2014/12/14
 *      Author: daisuke
 */

#include "TSVInputContext.h"

namespace RL
{


// 次のTokenを取得し現在のTokenを1つ進める
std::string TSVInputContext::nextToken()
{

	return "AAA";
}

// 次のTokenがskipStringであることを確認し現在のTokenを1つ進める
void TSVInputContext::skipToken(std::string skipString)
{

}

// 次のTokenが行末であることを確認し現在のTokenを次の行の初めに進める
void TSVInputContext::skipReturn(void)
{

}

// 現在の行で次のTokenがあるかどうかを確認する
bool TSVInputContext::hasNextToken(void)
{
	return false;
}

} /* namespace RL */
