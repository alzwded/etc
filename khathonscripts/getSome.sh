#!/bin/bash

if [[ $# < 2 ]] ; then
    echo $0 start end
fi

youtube-dl-2013 "${3:-$YOUTUBEDLLINK}" -f=18/43/5/17/34 -t -R 999 --playlist-start=$1 --playlist-end=$2
