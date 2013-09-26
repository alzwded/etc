#!/bin/bash

if [[ $# < 2 ]] ; then
    echo $0 start end
fi

youtube-dl-2013 'http://www.youtube.com/playlist?list=PL7EF8E7219F8F3C4F' --max-quality=18 -t -R 999 --playlist-start=$1 --playlist-end=$2