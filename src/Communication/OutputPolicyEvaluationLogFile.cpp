/*
 * OutputPolicyEvaluationLogFile.cpp
 *
 *  Created on: 2015/09/10
 *      Author: daisuke
 */

#include "OutputPolicyEvaluationLogFile.h"

#include <cmath>



namespace RL
{
void OutputPolicyEvaluationLogFile::process(OutputContext& output)
{
	//記述の簡略化のためのtypedef
	typedef PolicyEvaluationStatistics PES;
	//記述の簡略化のための参照定義
	const PES& pes = policyEvaluationStatistics;
	//記述の簡略化のための参照定義(Episode毎の統計量)
	const PES::TotalControlCostType& Sk = pes.totalControlCostAtEpisode;
	const PES::TotalControlCountType& Nk= pes.totalControlSelectCountAtEpisode;
	const PES::TotalControlCostType& SQk = pes.totalControlCostSqureAtEpisode;
	//記述の簡略化のための参照定義(全Episodeの統計量)
	const PES::TotalControlCostType& S = pes.totalControlCost;
	const PES::TotalControlCountType& N = pes.totalControlSelectCount;
	const PES::TotalControlCostType& SQ = pes.totalControlCostSqure;

	const real t_dist_95 =
			episodeIndex > 0 ?
					(episodeIndex > tDistributionMaxIndex ?
							tDistribution95PercentPoint[tDistributionMaxIndex] :
							tDistribution95PercentPoint[episodeIndex]) :
					0.0;
	const real t_dist_99 =
			episodeIndex > 0 ?
					(episodeIndex > tDistributionMaxIndex ?
							tDistribution99PercentPoint[tDistributionMaxIndex] :
							tDistribution99PercentPoint[episodeIndex]) :
					0.0;

	//episodeIndexの表示
	output.writeToken("episodeIndex");
	output.writeToken(std::to_string(episodeIndex));
	output.newLine();

	output.writeToken("state");
	output.writeToken("control");
	output.writeToken("total_cost_at_episode");
	output.writeToken("total_count_at_episode");
	output.writeToken("mean_cost_at_episode");
	output.writeToken("unbiased_variance_cost_at_episode");
	output.writeToken("total_cost");
	output.writeToken("total_count");
	output.writeToken("mean_cost");
	output.writeToken("unbiased_variance_cost");
	output.writeToken("95%_confidence_interval_min");
	output.writeToken("95%_confidence_interval_max");
	output.writeToken("99%_confidence_interval_min");
	output.writeToken("99%_confidence_interval_max");
	output.newLine();

	idx state_count = S.size();
	for(idx i = 0; i < state_count; i++)
	{
		idx control_count = S[i].size();
		for(idx u = 0; u < control_count; u++)
		{
			//Episode中のcostの平均
			real Mkiu= Sk[i][u]/Nk[i][u];
			//Episode中のcostの普遍分散
			real Vkiu=(SQk[i][u] - Nk[i][u] * (Mkiu * Mkiu)) /((real)(Nk[i][u]-1));
			//全Episodeのcostの平均
			real Miu= S[i][u]/N[i][u];
			//全Episodeのcostの普遍分散
			real Viu=(SQ[i][u] - N[i][u] * (Miu * Miu)) /((real)(N[i][u]-1));

			//母平均の95%区間の半分
			real diff_95 = t_dist_95 * std::sqrt((long double)(Viu / ((real)N[i][u])));
			//母平均の95%点（下限）
			real mean_95_min = Miu - diff_95;
			//母平均の95%点（上限）
			real mean_95_max = Miu + diff_95;

			//母平均の99%区間の半分
			real diff_99 = t_dist_99 * std::sqrt((long double)(Viu / ((real)N[i][u])));
			//母平均の99%点（下限）
			real mean_99_min = Miu - diff_99;
			//母平均の99%点（上限）
			real mean_99_max = Miu + diff_99;
			//////////////////
			output.writeToken(std::to_string(i));
			output.writeToken(std::to_string(u));
			output.writeToken(std::to_string(Sk[i][u]));
			output.writeToken(std::to_string(Nk[i][u]));
			output.writeToken(std::to_string(Mkiu));
			output.writeToken(std::to_string(Vkiu));
			output.writeToken(std::to_string(S[i][u]));
			output.writeToken(std::to_string(N[i][u]));
			output.writeToken(std::to_string(Miu));
			output.writeToken(std::to_string(Viu));
			//標本数が1の時は信頼区間を出力しない
			if(episodeIndex > 0)
			{
				output.writeToken(std::to_string(mean_95_min));
				output.writeToken(std::to_string(mean_95_max));
				output.writeToken(std::to_string(mean_99_min));
				output.writeToken(std::to_string(mean_99_max));
			}
			output.newLine();
		}
	}
}

void OutputPolicyEvaluationLogFile::ConstructTDist(void)
{
	real t_dist_95[] =
	{ 0.0,

			12.70620, 4.302653, 3.182446, 2.776445, 2.570582, 2.446912, 2.364624,
			2.306004, 2.262157, 2.228139,

			2.200985, 2.178813, 2.160369, 2.144787, 2.131450, 2.119905,
			2.109816, 2.100922, 2.093024, 2.085963,

			2.079614, 2.073873, 2.068658, 2.063899, 2.059539, 2.055529,
			2.051831, 2.048407, 2.045230, 2.042272,

			2.039513, 2.036933, 2.034515, 2.032245, 2.030108, 2.028094,
			2.026192, 2.024394, 2.022691, 2.021075,

			2.019541, 2.018082, 2.016692, 2.015368, 2.014103, 2.012896,
			2.011741, 2.010635, 2.009575, 2.008559 };

	real t_dist_99[] =
	{ 0.0,

			63.65674, 9.924843, 5.840909, 4.604095, 4.032143, 3.707428, 3.499483,
			3.355387, 3.249836, 3.169273,

			3.105807, 3.054540, 3.012276, 2.976843, 2.946713, 2.920782,
			2.898231, 2.878440, 2.860935, 2.845340,

			2.831360, 2.818756, 2.807336, 2.796940, 2.787436, 2.778715,
			2.770683, 2.763262, 2.756386, 2.749996,

			2.744042, 2.738481, 2.733277, 2.728394, 2.723806, 2.719485,
			2.715409, 2.711558, 2.707913, 2.704459,

			2.701181, 2.698066, 2.695102, 2.692278, 2.689585, 2.687013,
			2.684556, 2.682204, 2.679952, 2.677793 };

	tDistribution95PercentPoint.resize(tDistributionMaxIndex + 1);
	tDistribution99PercentPoint.resize(tDistributionMaxIndex + 1);
	for(idx f=0;f<tDistributionMaxIndex+1;f++)
	{
		tDistribution95PercentPoint[f] = t_dist_95[f];
		tDistribution99PercentPoint[f] = t_dist_99[f];
	}
}
} /* namespace RL */
