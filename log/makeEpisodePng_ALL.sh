#!/bin/bash
#EpsilonSoftOnPolicyMonteCarloのPolicyEvaluationのログファイルに対しgnuplotでpng形式のグラフを作成する
#引数にpng化したいログファイル名の先頭部分を指定すること
#ファイル名は少なくとも下記で始まるものであること
#EpsilonSoftOnPolicyMonteCarlo_PN2RL13EV3LineTracerE_PolicyEvaluation_
ls -1  $1*.log | awk '{print "basename " $$1 " .log";}' | bash | awk '{print "./makeEpisodeResultPng.sh "$$1;}' | bash
