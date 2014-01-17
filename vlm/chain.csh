#!/bin/csh

# Reduce the amount you need to type to set up your vlm server!
# Just run chain.csh HolidayVideo.m4v MySpeech.mp3 performance.avi
# and you're set!

onintr clean
hup 

if( $#argv < 1) then
    echo usage: $0 file1 file2 ... 
    exit 255
endif

set a=$0:h:q
[ -n $a:q ] && set a=$a:q/

set temp=`mktemp`

perl -w "$a"vlmcfggen.pl -$temp:q $*:q
bash "$a"vlm.sh $temp:q

clean:
rm $temp
