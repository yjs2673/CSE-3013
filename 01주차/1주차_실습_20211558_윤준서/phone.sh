#!/bin/bash

if [ $# -lt 1 ];then
	echo "Usage : phone searchfor [...searchfor]"
	echo "(You didn't tell me what you to earch for.)"
	exit 1 #end the script
fi

input="$1"

for i in $*
do
	input+="|"
	input+=$i
done
egrep -i $input data | awk -f display.awk
