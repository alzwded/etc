#!/bin/sh

if [ $# -lt 2 ] ; then
    echo usage: $0 fileToScrape extension
    exit 2
fi

expression='"https\?:\/\/.*\.'"$2"'"'

grep "$1" -e "$expression" | sed -e "s/.*\\($expression\\).*/\\1/g" | sed -e 's/^"\(.*\)"$/\1/g'
