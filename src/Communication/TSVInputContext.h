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
public:
	TSVInputContext(std::istream &is):position(0),separater('\t'),input(is),line()
	{
		//最初の行の読み込み
		newLine();
	}
	virtual ~TSVInputContext(){}

	// 次のTokenを取得し現在のTokenを1つ進める
	virtual std::string nextToken();

	// 次のTokenがskipStringであることを確認し現在のTokenを1つ進める
	virtual void skipToken(std::string skipString);

	// 次のTokenが行末であることを確認し現在のTokenを次の行の初めに進める
	virtual void skipReturn(void);

	// 現在の行で次のTokenがあるかどうかを確認する
	virtual bool hasNextToken(void);
};

} /* namespace RL */

#endif /* TSVINPUTCONTEXT_H_ */
