#!/bin/bash
echo $1
for i in `seq 1 $5`;
do
	time ./$1Sim > A$1_$2_$3_$4_$i
	sleep 10
done
