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
	static Random& GetInstance()
	{
		static Random instance;
		return instance;
	}
	//乱数の初期化を行う
	//(インスタンスが生成されていない場合は)コンストラクタが実行される
	static void Init()
	{
		GetInstance();
	}
	//疑似乱数を返す
	static int Get()
	{
		return rand();
	}
	//[0,max-1]の整数一様分布に従う疑似乱数を返す
	static int UniformIdx(int max)
	{
		return rand()%max;
	}
	//[0,1]の一様分布に従う疑似乱数を返す
	static real UniformReal()
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
	idx ValueMax;
	//整数の乱数に対する値を格納するvector
	std::vector<idx> Value;
	//確率の小数部に対する分布を表すvector
	std::vector<real> Decimal;
	//分割数
	idx PartitionCount;
	//各確率の小数部の和
	real DecimalTotal;
public:
	RandomIdx():
		ValueMax(0),
		PartitionCount(0),
		DecimalTotal(0.0)
	{
	};
	RandomIdx(const std::vector<real>& prob, idx part_count=16);
	real GetProbability(idx i)const
	{
		idx count=0;
		for(idx j=0;j<PartitionCount;j++)
		{
			if(Value[j]==i)
			{
				count++;
			}
		}
		return ((real)count)/((real)PartitionCount)+(Decimal[i+1]-Decimal[i]);
	}
	//指定された確率分布に従った疑似乱数を返す
	idx Get()const
	{
		//整数一様分布に従う疑似乱数を取得
		int x=Random::UniformIdx(PartitionCount);
		//xが整数部に適合するか確認
		if(Value[x]!=ValueMax)
		{
			//整数部のためValue[x]を返す
			return Value[x];
		}
		//xが小数部に適合する場合はreal値の一様分布から返却値を求める
		double d=DecimalTotal*Random::UniformReal();
		//線形探索で適合する区間を求める
		//この方法が遅い場合は2分探索を検討する
		//dが最も(数直線上で)左側の区間かその左側の場合0を返す
		if(d<Decimal[1])
		{
			return 0;
		}
		//(数直線上の)左側から順に区間に含まれるか確認し、含まれる区間のインデックスを返す
		for(idx i=1;i<Decimal.size()-1;i++)
		{
			if(d<Decimal[i+1])
			{
				return i;
			}
		}
		//(数直線上で)最も右側の区間の右側の場合は最大値を返す
		return ValueMax-1;
	}
	idx GetValueMax()const
	{
		return ValueMax;
	}
	bool operator==(const RandomIdx& org)const
	{
		if
		(
				(this->ValueMax==org.ValueMax)
				&&(this->PartitionCount==org.PartitionCount)
				&&(this->DecimalTotal==org.DecimalTotal)
				&&(this->Value==org.Value)
				&&(this->Decimal==org.Decimal)
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
