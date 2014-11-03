filename0=gmon_`date +%Y%m%d%H%M`
filename1=${filename0}_log.txt
filename=${filename0}.txt
./Debug/ReinforcementLearning > ${filename1}
gprof ./Debug/ReinforcementLearning gmon.out > ${filename}
#grep -F "`head -20 ${filename} | head -15 | tail -10 | sed -e "s/  */ /g" | cut -d " " -f 8-`" ${filename} | grep ^[[]
