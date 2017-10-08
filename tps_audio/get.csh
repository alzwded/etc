#!/bin/csh

set lastindex=`cat lastindex || echo 1`
set aistart=`expr $lastindex + 1`
set tps='https://www.youtube.com/channel/UCnUXq8mGmoHt0e6ItuTs10w'
set total=`./count_videos.pl $tps:q || echo 0`
set count=`expr $total - $lastindex`

youtube-dl -o "%(autonumber)s-%(title)s-%(id)s.%(ext)s" --autonumber-start=$aistart -f 139 --playlist-start=1 --playlist-end=$count $tps:q

./proc.csh

set msg="Don't forget to update lastindex to $total"
cowsay $msg || echo $msg
