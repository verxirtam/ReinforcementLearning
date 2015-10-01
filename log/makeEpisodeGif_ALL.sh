#!/bin/bash
#ファイル名が「EpsilonSoftOnPolicyMonteCarlo_PN2RL13EV3LineTracerE_PolicyEvaluation_[0-9]+」である連番pngファイル毎に、
#アニメーションgifファイルを作成する
ls -1 *.png | grep -o -E EpsilonSoftOnPolicyMonteCarlo_PN2RL13EV3LineTracerE_PolicyEvaluation_[0-9]+ | uniq | awk '{print "convert -delay 10 "$$1"*.png "$$1".gif"}' | bash
