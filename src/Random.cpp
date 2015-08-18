
/*
 * Random.cpp
 *
 *  Created on: 2014/01/12
 *      Author: daisuke
 */

#include <stdexcept>
#include "Random.h"



using namespace std;


namespace RL
{






RandomIdx::RandomIdx(const std::vector<real>& prob, idx part_count):
		valueMax(prob.size()),
		value(part_count),
		decimal(valueMax+1),
		partitionCount(part_count),
		decimalTotal()
{

	//part_countが不正な値なら例外発生
	if(part_count<1)
	{
		throw std::invalid_argument("error at RandomIdx::RandomIdx(). : part_count<1");
	}

	//乱数の初期化を行う
	Random::Init();

	vector<real> probability=prob;
	real sum=0.0;
	//Probability[]の値を正規化する
	//合わせて各値が正であるかチェックする（負なら例外発生）
	for(idx i=0;i<valueMax;i++)
	{
		if(probability[i]>=0.0)
		{
			sum+=probability[i];
		}
		else
		{
			//Probability[i]<0のため例外発生
			throw std::invalid_argument("error at RandomIdx::RandomIdx(). : probability[i]<0.0");
		}
	}
	//Probability[]の合計が0の時は例外発生
	if(sum==0.0)
	{
		throw std::invalid_argument("error at RandomIdx::RandomIdx(). : sum==0.0");
	}
	for(idx i=0;i<valueMax;i++)
	{
		probability[i]/=sum;
	}

	//確率を整数部(prob_int[])、小数部(prob_deci[])に分ける
	vector<idx> prob_int(valueMax);
	vector<real> prob_deci(valueMax);
	for(idx i=0;i<valueMax;i++)
	{
		prob_int[i]=(int)(probability[i]*partitionCount);
		prob_deci[i]=probability[i]-((real)prob_int[i])/((real)partitionCount);
	}

	//Value[],DecimalTotalの初期化
	idx p=0;
	for(idx i=0;i<valueMax;i++)
	{
		for(idx j=0;j<prob_int[i];j++)
		{
			value[p]=i;
			p++;
		}
	}
	decimalTotal=((real)partitionCount-p)/((real)partitionCount);
	for(;p<partitionCount;p++)
	{
		value[p]=valueMax;
	}

	//Decimal[]の初期化
	sum=0.0;
	decimal[0]=0.0;
	for(idx i=0;i<valueMax;i++)
	{
		sum+=prob_deci[i];
		decimal[i+1]=sum;
	}

}


}//namespace RL
