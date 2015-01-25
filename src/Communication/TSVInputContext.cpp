/*
 * TSVInputContext.cpp
 *
 *  Created on: 2014/12/14
 *      Author: daisuke
 */

#include "TSVInputContext.h"

using namespace std;

namespace RL
{

//1行分のstringからコメント(";"から行末まで)を削除
void TSVInputContext::deleteComment(const std::string& line, std::string& out)
{
	string::size_type i;
	//";"までの文字数を取得
	for (i = 0; i < line.length(); i++)
	{
		if (line[i] == ';')
		{
			break;
		}
	}
	//";"がない場合(i==line.length())は終了
	if(i==line.length())
	{
		//lineをそのまま返却
		out = line;
		return;
	}
	//iを";"(または行末)の1つ手前にする
	i--;
	//";"の手前のWSの1つ手前の位置を取得
	for (; i >= 0; i--)
	{
		if (line[i] != '\t')
		{
			break;
		}
	}
	//コメントが削除された行の文字数を取得
	int len = i + 1;
	//コメントが削除された行を取得
	out = line.substr(0, len);
}

//linestringのトークン分割を行う
void TSVInputContext::splitLineString(const string& linestring)
{
	string::size_type find = 0; //セパレータが見つかった位置
	//セパレータでトークン分割を行う
	for (int start = 0;	//現在位置のインデックス
	find != string::npos;	//セパレータが見つかる限りループ
			start = find + 1)
	{
		//現在位置から最初に現れるセパレータの位置を取得
		find = linestring.find_first_of(this->separater, start);
		//セパレータが見つかるかどうかで場合分け
		if (find != string::npos)
		{
			//セパレータが見つかった場合
			//現在位置からセパレータの手前までの文字列を取得
			line.push_back(linestring.substr(start, find - start));
		}
		else
		{
			//現在位置から行末までの文字列を取得
			line.push_back(linestring.substr(start));
		}
	}
}

//一行分の文字列の初期化
void TSVInputContext::newLine()
{

	//コメントを含む一行分の文字列
	string linestring_with_comment;
	//すでにEOFに達していたら例外発生
	if (input.eof())
	{
		//linestring_with_comment="";
		std::string msg("invalid file format ( unexpected EOF )");
		throw std::ios_base::failure(msg);
	}
	else
	{
		//入力ストリームから一行分を取得
		getline(input, linestring_with_comment);

		//ストリームからの取得に失敗した場合は例外発出(EOFに達していたかどうかは判定しない)
		if (input.bad())
		{
			std::string msg(
					"invalid file format ( can not read line ) : "
							+ linestring_with_comment);
			throw std::ios_base::failure(msg);
		}
	}
	//コメントを除いた一行分の文字列
	string linestring;
	//コメントを除いた行を取得
	deleteComment(linestring_with_comment, linestring);

	//現在行の内容をクリアする
	line.clear();

	//linestringのトークン分割を行う
	splitLineString(linestring);

	//位置を初期化
	position = 0;

}

// 次のTokenを取得し現在のTokenを1つ進める
std::string TSVInputContext::nextToken()
{
	// 現在位置が行末か確認
	if (!hasNextToken())
	{
		// 行末なら例外発生
		std::string msg("invalid file format ( end of line )");
		throw std::ios_base::failure(msg);
	}
	// 現在位置を1つ進める
	position++;
	// 進めた位置でのTokenを返却
	return line[position - 1];
}

// 現在のTokenがskipStringであることを確認し現在のTokenを1つ進める
void TSVInputContext::skipToken(const std::string skipString)
{
	// 次のTokenが無ければ例外発生
	if (!hasNextToken())
	{
		std::string msg("invalid file format ( end of line )");
		throw std::ios_base::failure(msg);
	}
	// 次のTokenがskipStringと一致して入れば現在位置を進める
	if (line[position] == skipString)
	{
		position++;
	}
	else
	{
		// 一致していない場合は例外発生
		std::string msg(
				"invalid file format ( unmatch to " + skipString + " )");
		throw std::ios_base::failure(msg);
	}

}

// 次のTokenが行末であることを確認し現在のTokenを次の行の先頭に進める
void TSVInputContext::skipReturn(void)
{
	//現在位置が行末かを確認
	if (position == line.size())
	{
		//行末なら次の行を読み込む
		this->newLine();
	}
	else
	{
		//行末でない場合は例外発出
		std::string msg("invalid file format ( not end of line )");
		throw std::ios_base::failure(msg);
	}
}

// 現在の行で次のTokenがあるかどうかを確認する
bool TSVInputContext::hasNextToken(void)
{
	return position != line.size();
}

} /* namespace RL */
