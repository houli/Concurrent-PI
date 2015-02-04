#!/bin/bash

simulations=$1

if [ ! -e 'mcserial' ]; then
  clang -o mcserial montecarloserial.c
fi

if [ ! -e 'mcparallel' ]; then
  clang -pthread -o mcparallel montecarloparallel.c
fi

touch results.txt

baseline=$({ /usr/bin/time -f %e ./mcserial $simulations >/dev/null; } 2>&1)
printf "%d %.3f\n" 1 $baseline >> results.txt

for i in {2..256}; do
  result=$({ /usr/bin/time -f %e ./mcparallel $simulations $i >/dev/null; } 2>&1)
  printf "%d %.3f\n" $i $result >> results.txt
done

gnuplot << EOF
set term png
set output "plot.png"
set title "$simulations Monte Carlo simulations"
set xlabel "Number of threads"
set ylabel "Time (seconds)"
plot "results.txt" using 1:2 with lines
EOF
