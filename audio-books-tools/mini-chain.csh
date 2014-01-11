#!/bin/csh

if($#argv < 2) then
    \echo "usage: $0 filename name"
    exit 1
endif

set file=$1
shift

set out=$1
shift

mkdir -p $out

./espeak.csh "$file" "$out/$out.wav" 
if($? == 0) then
    foreach i ("$out"/*)
        ffmpeg -i "$i" -acodec wmav2 -ab 32000 "$i:r.wma"
        if($? == 0) then
            rm "$i"
        endif
    end
endif
