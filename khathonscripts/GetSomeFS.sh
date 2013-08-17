#!/bin/bash

if [[ $# < 2 ]] ; then
    echo $0 start end
fi

youtube-dl-2013 'http://www.youtube.com/playlist?list=PL7EF8E7219F8F3C4F' --max-quality=43 -t -R 999 --playlist-start=$1 --playlist-end=$2

for i in *.webm ; do
    outFN="${i: -4}.mp4"
    ffmpeg -i "$i" -vcodec libx264 -acodec libmp3lame -ab 96000 "$outFN"
    if [ $? -eq 0 -a -f "$outFN" -a ` stat --printf="%s" "$outFN" ` -gt 4000 ] ; then
        rm -f "$i"
    fi
done
