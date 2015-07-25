/*
 * Episode.h
 *
 *  Created on: 2014/01/02
 *      Author: daisuke
 */

#ifndef EPISODE_H_
#define EPISODE_H_

#include "RLUtility.h"


namespace RL
{

//Episodeの1ステップ分の情報を表す構造体
struct Step
{
	idx State;
	idx Control;
	real Cost;

	inline bool operator==(const Step& org)const
	{
		return (this->State==org.State)
				&&(this->Control==org.Control)
				&&(this->Cost==org.Cost);
	}
	inline bool operator!=(const Step& org)const
	{
		return !((*this)==org);
	}

};



/*////////////////////////////////////////////////
 class Episode
 エピソードを表すクラス
 内部にstd::vector<Step>としてStepの情報を格納している。
 対応するMDPに適合しているかどうかはSet...()、Add...()実行時に判断しない。
 プログラマの責任でVerify()実行時に判断される。
 */////////////////////////////////////////////////
typedef std::vector<Step> EpisodeData;
class Episode
{
private:
	EpisodeData Steps;
public:
	//デフォルトコンストラクタ
	Episode():Steps()
	{

	}
	//コンストラクタ
	//EpisodeData
	Episode(EpisodeData ed):Steps(ed)
	{
	}
	//エピソードの長さを取得する
	inline idx GetStepCount()const
	{
		return Steps.size();
	}
	//指定した時点でのステップを取得する(読み取りのみ)
	inline const Step& operator[](idx i)const
	{
		return Steps[i];
	}
	//等号演算子
	inline bool operator==(const Episode& org)const
	{
		return (this->Steps)==(org.Steps);
	}
	//不等号演算子
	inline bool operator!=(const Episode& org)const
	{
		return !((*this) == org);
	}

	//対応するMDPと整合性が取れているかを確認し、
	//整合性が取れていない場合は例外を投げる
	//発生する例外：
	template <typename MDP>
	void Verify(const MDP& mdp)const
	{
		for(idx i;i<Steps.size();i++)
		{
			Episode::Verify(Steps[i],mdp);
		}
	}
	//引数で与えたStepが対応するMDPと整合性が取れているかを確認し、
	//整合性が取れていない場合は例外を投げる
	//発生する例外：TODO 未定。決めること。
	template <typename MDP>
	static void Verify(const Step& step,const MDP& mdp)
	{
		//TODO: MDP実装後に実装する
	}

	//指定したステップの設定を行う(Stepの内容を個別に指定)
	void SetStep(idx stepindex,idx state, idx control, real cost)
	{
		Step& step=Steps.at(stepindex);
		step.State=state;
		step.Control=control;
		step.Cost=cost;
	}
	//指定したステップの設定を行う(Stepで指定)
	void SetStep(idx stepindex,const Step& step)
	{
		Steps.at(stepindex)=step;
	}
	//ステップを追加する(Stepの内容を個別に指定)
	void AddStep(idx state, idx control, real cost)
	{
		Step step;
		step.State=state;
		step.Control=control;
		step.Cost=cost;
		this->AddStep(step);
	}
	//ステップを追加する(Stepで指定)
	void AddStep(const Step& step)
	{
		Steps.push_back(step);
	}
};

}//namespace RL



#endif /* EPISODE_H_ */
