#
#discountRate,episodeIndexを引数で指定すること
#
BEGIN{
}
{
	#項目数が1の行に達したらスクリプト終了
	if( NF == 1 )
	{
		exit
	}	
	#$1:stepIndex
	#$2:stateIndex
	#$3:controlIndex
	#$4:cost
	i=$2
	u=$3
	c=$4
	#訪問回数の算出
	count[i"\t"u]++
	
	#もともとのcostの算出(案0)
	cost = c + discountRate * cost
	calcCost[i"\t"u] += cost
	
	#案1のcostの算出
	#pre_iはエピソードを逆順に走査しているので
	#時間的には次のステップでのstateになる
	plan1Cost = pre_i + discountRate * plan1Cost
	plan1CalcCost[i"\t"u] += plan1Cost
	
	#案2のコストの算出
	plan2Cost = pre_i * pre_i + discountRate * plan2Cost
	plan2CalcCost[i"\t"u] += plan2Cost
	
	#案3のコストの算出
	plan3LocalCost = pre_i > 2 ? 1 : 0
	plan3Cost = plan3LocalCost + discountRate * plan3Cost
	plan3CalcCost[i"\t"u] += plan3Cost
	
	#案4のコストの算出
	plan4LocalCost = pre_i > 2 ? 1 : 0.125
	plan4Cost = plan4LocalCost + discountRate * plan4Cost
	plan4CalcCost[i"\t"u] += plan4Cost

	#案5のコストの算出
	#stateに応じたコストと、
	#ゴール時の経過時間に応じたコスト(もともとのコスト)を加算
	plan5LocalCost = pre_c + ( pre_i > 2 ? 1 : 0.125 )
	plan5Cost = plan5LocalCost + discountRate * plan5Cost
	plan5CalcCost[i"\t"u] += plan5Cost

	#次のステップのための初期化
	pre_i=i
	pre_u=u
	pre_c=c
}
END{
	for( iu in calcCost )
	{
		print discountRate "\t" episodeIndex "\t" iu "\t" 0 "\t" count[iu] "\t" calcCost[iu]
	       	print discountRate "\t" episodeIndex "\t" iu "\t" 1 "\t" count[iu] "\t" plan1CalcCost[iu]
		print discountRate "\t" episodeIndex "\t" iu "\t" 2 "\t" count[iu] "\t" plan2CalcCost[iu] 
		print discountRate "\t" episodeIndex "\t" iu "\t" 3 "\t" count[iu] "\t" plan3CalcCost[iu] 
		print discountRate "\t" episodeIndex "\t" iu "\t" 4 "\t" count[iu] "\t" plan4CalcCost[iu] 
		print discountRate "\t" episodeIndex "\t" iu "\t" 5 "\t" count[iu] "\t" plan5CalcCost[iu] 
	}

}
