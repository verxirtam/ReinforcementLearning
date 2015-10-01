#!/bin/bash
gnuplot << EOF
set terminal png size 1280,960
set out "$1.png"
set xrange [0:5]
set yrange [0:70]
plot "$1.log" using (\$1+\$2*0.1):(\$9):(\$11):(\$12) with errorbars
EOF
