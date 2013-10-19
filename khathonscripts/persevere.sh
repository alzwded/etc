#!/bin/bash

sleepAmount=20
try=1

while true ; do
    echo try \#$try
    try=`expr $try + 1`
	./getSome.sh $1 $2 2>&1 | grep ERROR
    if [[ $?  != 0 ]] ; then
        echo no error
        break
    fi
    echo sleeping $sleepAmount
    sleep $sleepAmount
	if [[ $? != 0 ]] ; then
        echo interrupted by l-user
		break
	fi
done
