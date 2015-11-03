BEGIN{
	discountRate = 1.000000
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
	#もともとのcostの算出
	cost = c + discountRate * cost
	calcCost[i"\t"u] += cost
	#案1のcostの算出
	plan1Cost = 0.01 + discountRate * plan1Cost
	plan1CalcCost[i"\t"u] += plan1Cost
	#案2のコストの算出
	#pre_iはエピソードを逆順に走査しているので
	#時間的には次のステップでのstateになる
	plan2Cost = pre_i + discountRate * plan2Cost
	plan2CalcCost[i"\t"u] += plan2Cost
	#案3のコストの算出
	plan3LocalCost = pre_i > 2 ? 1 :0
	plan3Cost = plan3LocalCost + discountRate * plan3Cost
	plan3CalcCost[i"\t"u] += plan3Cost
	
	#案4のコストの算出
	plan4Cost = pre_i * pre_i + discountRate * plan4Cost
	plan4CalcCost[i"\t"u] += plan4Cost

	#次のステップのための初期化
	pre_i=i
	pre_u=u
	pre_c=c
}
END{
	for( iu in calcCost )
	{
		print iu "\t" count[iu] "\t" calcCost[iu] "\t" plan1CalcCost[iu] "\t" plan2CalcCost[iu] "\t" plan3CalcCost[iu] "\t" plan4CalcCost[iu]
	}

}
