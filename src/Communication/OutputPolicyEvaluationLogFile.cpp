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
	const PES::TotalControlCountType& K = pes.totalControlSelectEpisodeCount;
	const PES::TotalControlCostType& SQ = pes.totalControlCostSqure;
	const PES::TotalControlCountType& NQ = pes.totalControlSelectCountSqure;


	//episodeIndexの表示
	output.writeToken("episodeIndex");
	output.writeToken(std::to_string(episodeIndex));
	output.newLine();

	//episodeIndexの表示
	output.writeToken("epsilon");
	output.writeToken(std::to_string(epsilon));
	output.newLine();

	output.writeToken("01_state");
	output.writeToken("02_control");
	output.writeToken("03_total_cost_at_episode");
	output.writeToken("04_total_count_at_episode");
	output.writeToken("05_mean_cost_at_episode");
	output.writeToken("06_unbiased_variance_cost_at_episode");
	output.writeToken("07_total_select_episode_count");
	output.writeToken("08_total_cost");
	output.writeToken("09_total_count");
	output.writeToken("10_total_mean_cost_sum");
	output.writeToken("11_total_unbiased_variance_cost_sum");
	output.writeToken("12_total_mean_count");
	output.writeToken("13_total_unbiased_variance_count");
	output.writeToken("14_mean_cost");
	output.writeToken("15_95%_confidence_interval_min");
	output.writeToken("16_95%_confidence_interval_max");
	output.writeToken("17_99%_confidence_interval_min");
	output.writeToken("18_99%_confidence_interval_max");
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
			real Uk2iu=(SQk[i][u] - Nk[i][u] * (Mkiu * Mkiu)) /((real)(Nk[i][u]-1));
			//全Episodeのcostの平均
			real Miu= S[i][u]/N[i][u];

			//自由度
			idx freedom = K[i][u]-1;
			//t分布の95%点
			const real t_dist_95 =
					freedom > 0 ?
							(freedom > tDistributionMaxIndex ?
									tDistribution95PercentPoint[tDistributionMaxIndex] :
									tDistribution95PercentPoint[freedom]) :
							0.0;
			//t分布の99%点
			const real t_dist_99 =
					freedom > 0 ?
							(freedom > tDistributionMaxIndex ?
									tDistribution99PercentPoint[tDistributionMaxIndex] :
									tDistribution99PercentPoint[freedom]) :
							0.0;
			/////////////////////
			//Skの平均
			real MSiu = S[i][u]/((real)(K[i][u]));
			//Skの不偏分散
			real US2iu=SQ[i][u]/((real)(K[i][u]-1))-S[i][u]*S[i][u]/((real)(K[i][u]*(K[i][u]-1)));


			//Sの95%信頼区間の幅の半分
			real Siu_95_width_half = t_dist_95 * std::sqrt((long double)(US2iu/((real)(K[i][u]))));
			//Sの99%信頼区間の幅の半分
			real Siu_99_width_half = t_dist_99 * std::sqrt((long double)(US2iu/((real)(K[i][u]))));

			//Sの95%信頼区間の最小値
			real Siu_95_min = MSiu - Siu_95_width_half;
			//Sの95%信頼区間の最大値
			real Siu_95_max = MSiu + Siu_95_width_half;
			//Sの99%信頼区間の最小値
			real Siu_99_min = MSiu - Siu_99_width_half;
			//Sの99%信頼区間の最大値
			real Siu_99_max = MSiu + Siu_99_width_half;
			/////////////////////
			//Nkの平均
			real MNiu = ((real)(N[i][u]))/((real)(K[i][u]));
			//Nkの不偏分散
			real UN2iu=((real)(NQ[i][u]))/((real)(K[i][u]-1))-((real)(N[i][u]*N[i][u]))/((real)(K[i][u]*(K[i][u]-1)));


			//Nの95%信頼区間の幅の半分
			real Niu_95_width_half = t_dist_95 * std::sqrt((long double)(UN2iu/((real)(K[i][u]))));
			//Nの99%信頼区間の幅の半分
			real Niu_99_width_half = t_dist_99 * std::sqrt((long double)(UN2iu/((real)(K[i][u]))));

			//Nの95%信頼区間の最小値
			real Niu_95_min = MNiu - Niu_95_width_half;
			//Nの95%信頼区間の最大値
			real Niu_95_max = MNiu + Niu_95_width_half;
			//Nの99%信頼区間の最小値
			real Niu_99_min = MNiu - Niu_99_width_half;
			//Nの99%信頼区間の最大値
			real Niu_99_max = MNiu + Niu_99_width_half;
			////////////////////////

			//価値関数の95%信頼区間の最小値
			real Miu_95_min = Siu_95_min / Niu_95_max;
			//価値関数の95%信頼区間の最大値
			real Miu_95_max = Siu_95_max / Niu_95_min;

			//価値関数の99%信頼区間の最小値
			real Miu_99_min = Siu_99_min / Niu_99_max;
			//価値関数の99%信頼区間の最大値
			real Miu_99_max = Siu_99_max / Niu_99_min;


			//////////////////
			output.writeToken(std::to_string(i));
			output.writeToken(std::to_string(u));
			output.writeToken(std::to_string(Sk[i][u]));
			output.writeToken(std::to_string(Nk[i][u]));
			output.writeToken(std::to_string(Mkiu));
			output.writeToken(std::to_string(Uk2iu));
			output.writeToken(std::to_string(K[i][u]));
			output.writeToken(std::to_string(S[i][u]));
			output.writeToken(std::to_string(N[i][u]));
			output.writeToken(std::to_string(MSiu));
			output.writeToken(std::to_string(US2iu));
			output.writeToken(std::to_string(MNiu));
			output.writeToken(std::to_string(UN2iu));
			output.writeToken(std::to_string(Miu));
			output.writeToken(std::to_string(Miu_95_min));
			output.writeToken(std::to_string(Miu_95_max));
			output.writeToken(std::to_string(Miu_99_min));
			output.writeToken(std::to_string(Miu_99_max));
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
