#!/bin/csh

if($#argv < 2) then
    \echo "usage: $0 filename name"
    exit 1
endif

set file="$1"
shift

set out="$1"
shift

mkdir -p $out:q

csh ./espeak.csh "$file" "$out/$out.wav" 
if($? == 0) then
    set idx=1
    foreach i ("$out"/*)
        #ffmpeg -i "$i" -acodec wmav2 -ab 32000 "$i:r.wma"
        #set ofile="$i:r.ogg"
        #ffmpeg -i "$i" -acodec libvorbis -ab 32000 "$ofile"
        set ofile="$i:r.mp3"
        avconv -i "$i" -ab 32000 "$ofile"
        #set ofile="$i:r.mp4"
        #ffmpeg -i "$i" -acodec aac -ab 32000 "$ofile"
        if($? == 0) then
            rm "$i"
            id3v2 -a "$out" -A "$out" -t "$ofile:t" -T "$idx" "$ofile"
            @ idx ++
        endif
    end
endif
