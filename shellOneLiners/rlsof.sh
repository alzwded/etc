#!/bin/bash

function prnt {
    set rez=`lsof "$1"`
    [ $? -ne 0 -a -n "$rez" ] && echo "lsof of $1:" && echo "$rez" && echo "--"
}

function f {
    echo entering $1
    for i in "$1"/* ; do
        [ -d "$i" ] && f "$i" || prnt "$i"
    done
}

f Dropbox
