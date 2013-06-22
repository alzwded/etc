#!/bin/bash

if [[ $# < 1 ]] ; then
    cmdLine=1
    pid=1
else
    while [[ $# > 0 ]]; do
        if [[ "$1" == 'cmdLine' ]] ; then
            cmdLine=1
        elif [[ "$1" == 'pid' ]] ; then
            pid=1
        elif [[ "$1" == '-h' ]] ; then
            echo "prints the pid and the command line of the process that created a window"
            echo "usage: $0 [cmdLine] [pid]"
            exit 255
        else
            echo 'unknown parameter '$1
            exit 1
        fi
        shift
    done
fi

pidOfWindow=`xprop _NET_WM_PID | cut -f 3 -d ' '`
[ $? -ne 0 ] && exit 1
[ ${pid:-0} -ne 0 ] && echo $pidOfWindow
[ ${cmdLine:-0} -ne 0 ] && echo `cat /proc/$pidOfWindow/cmdline`
