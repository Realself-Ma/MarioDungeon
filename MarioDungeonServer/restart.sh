#! /bin/bash

while true 
do
	monitor=`ps -ef | grep main | grep -v grep | wc -l ` 
	if [ $monitor -eq 0 ] ;
	then
		echo "MarioServer program is not running, restart it"
		./main 2021 
	#else
		#echo "MarioServer program is running"
	fi
	sleep 1
done																		
