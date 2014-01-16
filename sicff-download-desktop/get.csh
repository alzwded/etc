#!/bin/tcsh

set dir=$0:h
[ -n $dir ] && set dir=$dir:q/
foreach i (*.desktop)
    set l=`perl "$dir:q"downloader.pl $i:q`
    wget -t INF --user="aut" --password="aut" $l:q
end
