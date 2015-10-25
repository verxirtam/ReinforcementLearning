#!/bin/bash


#コストの算出案でコストを算出する
#出力先：EV3LineTracer_20151021232930_20Episode_YYYYMMDDhhmm_calc_cost.txt
#YYYYMMDDhhmm：エピソードを識別するエピソード開始年月日時分秒
#各案の算出結果が列ごとに格納される
ls -1 *20Episode*.log | sort | awk '{filename=$1;sub(/\.log$/,"",filename);print "tac "$1" | awk -f calc_cost.awk > "filename"_calc_cost.txt"}' | bash

#上記で算出したコストはエピソード単位にファイルがわかれているのでコントロール毎に各エピソードの値を集約する。
#出力先ファイル：calc_cost_<i>_<u>.txt
#i:state index
#u:control index
cat EV3LineTracer_20151021232930_20Episode_20151022050017_calc_cost.txt | awk '{print "cat *calc_cost.txt | grep -P \"^"$1"\\t"$2"\" > calc_cost_"$1"_"$2".txt"}' | bash

#案とコントロール毎に信頼区間の推移を算出する
#案を増やした時はplanの番号と3行目のawkの内部のawkのprint文にある4列表示している3つ目を変更する
#        awk "{print \"cat \"\$1\" | awk '{print \$1,\$2,\$4,\$3}' | awk -f ../accum.awk > \"\$1\".plan0\"}" |
#	                                                   ^ここを変更する                             ^ここを変更する
cat EV3LineTracer_20151021232930_20Episode_20151022050017_calc_cost.txt |\
	awk "{print \"calc_cost_\"\$1\"_\"\$2\".txt\"}" |\
	awk "{print \"cat \"\$1\" | awk '{print \$1,\$2,\$4,\$3}' | awk -f ../accum.awk > \"\$1\".plan0\"}" |\
	bash
cat EV3LineTracer_20151021232930_20Episode_20151022050017_calc_cost.txt |\
      	awk "{print \"calc_cost_\"\$1\"_\"\$2\".txt\"}" |\
       	awk "{print \"cat \"\$1\" | awk '{print \$1,\$2,\$5,\$3}' | awk -f ../accum.awk > \"\$1\".plan1\"}" |\
       	bash
cat EV3LineTracer_20151021232930_20Episode_20151022050017_calc_cost.txt |\
        awk "{print \"calc_cost_\"\$1\"_\"\$2\".txt\"}" |\
        awk "{print \"cat \"\$1\" | awk '{print \$1,\$2,\$6,\$3}' | awk -f ../accum.awk > \"\$1\".plan2\"}" |\
        bash
cat EV3LineTracer_20151021232930_20Episode_20151022050017_calc_cost.txt |\
        awk "{print \"calc_cost_\"\$1\"_\"\$2\".txt\"}" |\
        awk "{print \"cat \"\$1\" | awk '{print \$1,\$2,\$7,\$3}' | awk -f ../accum.awk > \"\$1\".plan3\"}" |\
        bash
cat EV3LineTracer_20151021232930_20Episode_20151022050017_calc_cost.txt |\
        awk "{print \"calc_cost_\"\$1\"_\"\$2\".txt\"}" |\
        awk "{print \"cat \"\$1\" | awk '{print \$1,\$2,\$8,\$3}' | awk -f ../accum.awk > \"\$1\".plan4\"}" |\
        bash


#ステップ100時点の信頼区間を案ごとに集約する
ls -1 calc_cost_*plan0 | awk '{print "head -100 "$1" | tail -1"}' | bash > step100_plan0.txt
ls -1 calc_cost_*plan1 | awk '{print "head -100 "$1" | tail -1"}' | bash > step100_plan1.txt
ls -1 calc_cost_*plan2 | awk '{print "head -100 "$1" | tail -1"}' | bash > step100_plan2.txt
ls -1 calc_cost_*plan3 | awk '{print "head -100 "$1" | tail -1"}' | bash > step100_plan3.txt
ls -1 calc_cost_*plan4 | awk '{print "head -100 "$1" | tail -1"}' | bash > step100_plan4.txt



