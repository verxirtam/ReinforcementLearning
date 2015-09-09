/*
 * OutputPolicyEvaluationLogFile.cpp
 *
 *  Created on: 2015/09/10
 *      Author: daisuke
 */

#include "OutputPolicyEvaluationLogFile.h"



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


	idx state_count = S.size();
	for(idx i = 0; i < state_count; i++)
	{
		idx control_count = S[i].size();
		for(idx u = 0; u < control_count; u++)
		{
			//Episode中のcostの平均
			real Mkiu= Sk[i][u]/Nk[i][u];
			//Episode中のcostの普遍分散
			real Vkiu=(SQk[i][u]/Nk[i][u] - (Mkiu * Mkiu)) * (Nk[i][u])/(Nk[i][u]-1);
			//全Episodeのcostの平均
			real Miu= S[i][u]/N[i][u];
			//全Episodeのcostの普遍分散
			real Viu=(SQ[i][u]/N[i][u] - (Miu * Miu)) * (N[i][u])/(N[i][u]-1);
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
			output.newLine();
		}
	}
}
} /* namespace RL */
