#!/bin/bash

#calc_cost.txtの作成
#">>"を使用しているので事前にファイルを削除する必要がある。
rm calc_cost.txt

for discountRate in 0.0 0.25 0.5 0.9375 1.0
do
	ls -1 *20Episode*.log | sort | awk '{filename=$1;sub(/\.log$/,"",filename);print "tac "$1" | awk  -f calc_cost.awk -v discountRate='$discountRate' -v episodeIndex="(NR-1)" >> calc_cost.txt"}' | bash
done
#
#discountRate	episodeIndex	i	u	p	count[i][u]	plan[p]CalcCost[i][u]
#


#join用のファイル作成
awk '{print $1":"$2":"$3":"$4":"$5"\t"$0}' calc_cost.txt | sort > calc_cost_forJoin.txt
#
#discountRate:episodeIndex:i:u:p	discountRate	episodeIndex	i	u	p	count[i][u]	plan[p]CalcCost[i][u]
#



#可能な(discountRate,i,u,p)を列挙したファイルを作成する
cat calc_cost.txt | cut -f 1,3,4,5 | sort | uniq > index_discountRate_i_u_p.txt


#各案の信頼区間の出力
#(discountRate,i,u,p)に関してループ
#下記は（0.25,2,1,4）の場合
#grep -P "^0\.25\t[0-9]+\t2\t1\t4" calc_cost.txt | awk '{print $3,$4,$7,$6}' | awk -f ../accum.awk | awk '{print "0.25""\t"(NR-1)"\t"2"\t"1"\t"4"\t"$0}' > confInt_025000_plan4.txt

rm confInt.txt
cat index_discountRate_i_u_p.txt | while read line
do
	#$lineを4つの変数$discountRate, $i, $u, $pに分解する
	set -- $line
	discountRate=$1
	i=$2
	u=$3
	p=$4
	grep -P "^"$discountRate"\t[0-9]+\t"$i"\t"$u"\t"$p calc_cost.txt | awk '{print $3,$4,$7,$6}' | awk -f ../accum.awk | awk '{print "'$discountRate'""\t"(NR-1)"\t'$i'\t'$u'\t'$p'\t"$0}'>> confInt.txt
done

#
#<confInt.txt>
#discountRate	episodeIndex	i	u	p	plan[p]_costMean[i][u]	plan[p]_confInt95%PointMin[i][u]	plan[p]_confInt95%PointMax[i][u]
#



#join用のファイル作成
awk '{print $1":"$2":"$3":"$4":"$5"\t"$0}' confInt.txt | sort > confInt_forJoin.txt
#
#<confInt_forJoin.txt>
#discountRate:episodeIndex:i:u:p	discountRate	episodeIndex	i	u	p	plan[p]_costMean[i][u]	plan[p]_confInt95%PointMin[i][u]	plan[p]_confInt95%PointMax[i][u]
#


#joinして結果出力
join -a 1 -t "$(printf '\011')" calc_cost_forJoin.txt confInt_forJoin.txt | cut -f 2,3,4,5,6,7,8,14,15,16 > calc_cost_result.txt

#<calc_cost_result.txt>
#discountRate	episodeIndex	i	u	p	count[i][u]	plan[p]CalcCost[i][u]	plan[p]_costMean[i][u]	plan[p]_confInt95%PointMin[i][u]	plan[p]_confInt95%PointMax[i][u]

