#!/bin/tcsh

set dir=$0:h
[ -n $dir:q ] && set dir=$dir:q/
foreach i (*.desktop)
    set l=`perl "$dir:q"downloader.pl $i:q`
    [ -n $l:q ] && wget -c -t INF --user="aut" --password="aut" $l:q || echo skipping $i 'because I could not find a URL=[...] line'
end
