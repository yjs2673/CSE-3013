#!/bin/bash

echo "working directory :"
read directory

if [ -e $directory ]; then
	cd $directory || exit
	for file in *; do
		newname=`echo $file | tr "[a-z] [A-Z]" "[A-Z] [a-z]"`
		mv $file $newname
	done
else
	echo "error message"
fi
