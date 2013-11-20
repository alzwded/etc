#!/bin/sh

CONVERTER=`[ -x /usr/bin/avconv ] && echo avconv || echo ffmpeg`

for i in "$@"
do 
    $CONVERTER -i "$i" -map 0:0 -map 0:1 -acodec copy -vcodec copy "$i.singleTrack.mp4"
    if [ $? -eq 0 -a -f "$i.singleTrack.mp4" -a `stat -c %s "$i.singleTrack.mp4"` -gt 4096 ]
    then
        rm "$i"
    else
        echo cataclysmic error
        exit 3
    fi
done
