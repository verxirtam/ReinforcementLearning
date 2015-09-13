/*
 * PolicyEvaluationStatistics.h
 *
 *  Created on: 2015/09/13
 *      Author: daisuke
 */

#ifndef POLICYEVALUATIONSTATISTICS_H_
#define POLICYEVALUATIONSTATISTICS_H_

#include <stdexcept>
#include <string>
#include <vector>

#include "RLUtility.h"

namespace RL
{

class PolicyEvaluationStatistics
{
public:
	typedef std::vector<std::vector<real> > TotalControlCostType;
	typedef std::vector<std::vector< idx> > TotalControlCountType;

	const TotalControlCostType& totalControlCostAtEpisode;//Sk;//Episode中のcontrolのcostの合計
	const TotalControlCountType& totalControlSelectCountAtEpisode;//Nk;//Episode中のcontrolを選択した回数
	const TotalControlCostType& totalControlCostSqureAtEpisode;//SQk;//Episode中のcontrolのcost^2の合計
	const TotalControlCostType& totalControlCost;//S;//controlのcostの合計
	const TotalControlCountType& totalControlSelectCount;//N;//controlを選択した回数
	const TotalControlCostType& totalControlCostSqure;//SQ;//controlのcost^2の合計

	PolicyEvaluationStatistics(
			const TotalControlCostType& totalControlCostAtEpisode_,//Sk;//Episode中のcontrolのcostの合計
			const TotalControlCountType& totalControlSelectCountAtEpisode_,//Nk;//Episode中のcontrolを選択した回数
			const TotalControlCostType& totalControlCostSqureAtEpisode_,//SQk;//Episode中のcontrolのcost^2の合計
			const TotalControlCostType& totalControlCost_,//S;//controlのcostの合計
			const TotalControlCountType& totalControlSelectCount_,//N;//controlを選択した回数
			const TotalControlCostType& totalControlCostSqure_//SQ;//controlのcost^2の合計
	):
		totalControlCostAtEpisode(totalControlCostAtEpisode_),
		totalControlSelectCountAtEpisode(totalControlSelectCountAtEpisode_),
		totalControlCostSqureAtEpisode(totalControlCostSqureAtEpisode_),
		totalControlCost(totalControlCost_),
		totalControlSelectCount(totalControlSelectCount_),
		totalControlCostSqure(totalControlCostSqure_)
	{
		//各メンバの要素数が一致していることを確認する
		idx state_count = totalControlCostAtEpisode.size();
		if ((state_count != totalControlSelectCountAtEpisode.size())
				|| (state_count != totalControlCostSqureAtEpisode.size())
				|| (state_count != totalControlCost.size())
				|| (state_count != totalControlSelectCount.size())
				|| (state_count != totalControlCostSqure.size()))
		{
			throw std::range_error("state_countが一致しません\n");
		}
		for(idx i = 0; i < state_count; i++)
		{
			idx control_count = totalControlCostAtEpisode[i].size();
			if ((control_count != totalControlSelectCountAtEpisode[i].size())
					|| (control_count
							!= totalControlCostSqureAtEpisode[i].size())
					|| (control_count != totalControlCost[i].size())
					|| (control_count != totalControlSelectCount[i].size())
					|| (control_count != totalControlCostSqure[i].size()))
			{
				throw std::range_error(
						std::string("") + "control_count[" + std::to_string(i)
								+ "]が一致しません\n");
			}
		}
	}
};


} /* namespace RL */

#endif /* POLICYEVALUATIONSTATISTICS_H_ */
