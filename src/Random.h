/*
 * Random.h
 *
 *  Created on: 2014/01/12
 *      Author: daisuke
 */

#ifndef RANDOM_CPP_
#define RANDOM_CPP_

#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <pthread.h>
#include "RLUtility.h"



namespace RL
{

//乱数を扱うクラス
//srand()をプログラム中1回だけ使用するため
//Singletonパターンを適用している
class Random
{
private:
	//Singletonパターンのため、
	//コンストラクタ、コピーコンストラクタ、コピー代入演算子をprivateにする
	Random()
	{
		//乱数の初期化
		srand((unsigned int)time(NULL));
	}
	Random(const Random& r){}
	Random& operator=(const Random& r){return *this;}
public:
	//唯一のインスタンスを取得する
	static Random& getInstance()
	{
		static Random instance;
		return instance;
	}
	//乱数の初期化を行う
	//(インスタンスが生成されていない場合は)コンストラクタが実行される
	static void Init()
	{
		getInstance();
	}
	//疑似乱数を返す
	static int get()
	{
		return rand();
	}
	//[0,max-1]の整数一様分布に従う疑似乱数を返す
	static int uniformIdx(int max)
	{
		return rand()%max;
	}
	//[0,1]の一様分布に従う疑似乱数を返す
	static real uniformReal()
	{
		return ((real)rand())/((real)RAND_MAX);
	}
};

//////////////////////////////////////////////////////////////////////////

//指定した確率で[0,n]の整数を返す疑似乱数
class RandomIdx
{
private:
	//Valueの値の最大値
	//(Probability[]のサイズ)
	idx valueMax;
	//整数の乱数に対する値を格納するvector
	std::vector<idx> value;
	//確率の小数部に対する分布を表すvector
	std::vector<real> decimal;
	//分割数
	idx partitionCount;
	//各確率の小数部の和
	real decimalTotal;
public:
	RandomIdx():
		valueMax(0),
		value(),
		decimal(),
		partitionCount(0),
		decimalTotal(0.0)
	{
	};
	RandomIdx(const std::vector<real>& prob, idx part_count=16);
	real getProbability(idx i)const
	{
		idx count=0;
		for(idx j=0;j<partitionCount;j++)
		{
			if(value[j]==i)
			{
				count++;
			}
		}
		return ((real)count)/((real)partitionCount)+(decimal[i+1]-decimal[i]);
	}
	//指定された確率分布に従った疑似乱数を返す
	idx get()const
	{
		//整数一様分布に従う疑似乱数を取得
		int x=Random::uniformIdx(partitionCount);
		//xが整数部に適合するか確認
		if(value[x]!=valueMax)
		{
			//整数部のためValue[x]を返す
			return value[x];
		}
		//xが小数部に適合する場合はreal値の一様分布から返却値を求める
		double d=decimalTotal*Random::uniformReal();
		//線形探索で適合する区間を求める
		//この方法が遅い場合は2分探索を検討する
		//dが最も(数直線上で)左側の区間かその左側の場合0を返す
		if(d<decimal[1])
		{
			return 0;
		}
		//(数直線上の)左側から順に区間に含まれるか確認し、含まれる区間のインデックスを返す
		for(idx i=1;i<decimal.size()-1;i++)
		{
			if(d<decimal[i+1])
			{
				return i;
			}
		}
		//(数直線上で)最も右側の区間の右側の場合は最大値を返す
		return valueMax-1;
	}
	idx getValueMax()const
	{
		return valueMax;
	}
	bool operator==(const RandomIdx& org)const
	{
		if
		(
				(this->valueMax==org.valueMax)
				&&(this->partitionCount==org.partitionCount)
				&&(this->decimalTotal==org.decimalTotal)
				&&(this->value==org.value)
				&&(this->decimal==org.decimal)
		)
		{
			return true;
		}
		return false;
	}
	bool operator!=(const RandomIdx& org)const
	{
		return !((*this)==org);
	}
};

}

#endif /* RANDOM_CPP_ */
