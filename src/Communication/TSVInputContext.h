/*
 * TSVInputContext.h
 *
 *  Created on: 2014/12/14
 *      Author: daisuke
 */

#ifndef TSVINPUTCONTEXT_H_
#define TSVINPUTCONTEXT_H_

#include <istream>
#include <string>
#include <vector>

#include "InputContext.h"

namespace RL
{

class TSVInputContext: public InputContext
{
private:
	//読み込み開始したかを表す
	//最初のnewLine()を行うかの判断に利用する
	bool isBeginning;
	//行中の位置を表す
	std::vector<std::string>::size_type position;
	//区切り文字を表す
	char separater;
	//入力ストリーム
	std::istream &input;
	//1行分の文字列を区切り文字で分割したトークンのリスト
	std::vector<std::string> line;
	//1行分のstringからコメント(";"の手前のWSから行末まで)を削除
	void deleteComment(const std::string& line, std::string& out);
	//一行分の文字列の初期化
	void newLine();
	void splitLineString(const std::string& linestring);
	//読み込み開始のため最初の行を読み込む
	//読み込み開始済みなら何もしない
	void begin()
	{
		//読み込み前の場合は最初の行を読み込む
		if(isBeginning)
		{
			//最初の行を読み込む
			newLine();
			//isBeginningを読み込み開始済みに設定する
			isBeginning=false;
		}
	}
	// 現在の行で次のTokenがあるかどうかを確認する
	// isBeginningのチェックをしない
	bool hasNextTokenPrivate(void)
	{
		return position != line.size();
	}

public:
	TSVInputContext(std::istream &is):isBeginning(true),position(0),separater('\t'),input(is),line()
	{
	}
	virtual ~TSVInputContext(){}

	// 次のTokenを取得し現在のTokenを1つ進める
	virtual std::string nextToken();

	// 次のTokenがskipStringであることを確認し現在のTokenを1つ進める
	virtual void skipToken(const std::string skipString);

	// 次のTokenが行末であることを確認し現在のTokenを次の行の初めに進める
	virtual void skipReturn(void);

	// 現在の行で次のTokenがあるかどうかを確認する
	// isBeginningのチェックを行う
	virtual bool hasNextToken(void)
	{
		//読み込み前の場合は最初の行を読み込む
		begin();
		return hasNextTokenPrivate();
	}
	//入力バッファと同期を取る
	virtual void sync()
	{
		input.sync();
	}

};

} /* namespace RL */

#endif /* TSVINPUTCONTEXT_H_ */
