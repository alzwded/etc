#!/bin/csh

if($#argv < 2) then
    \echo "usage: $0 filename name"
    exit 1
endif

set file="$1"
shift

set out="$1"
shift

set basePath=$out:h:q
set tailPath=$out:t:q

set dirPath=$basePath:q/$tailPath:q
set wavFiles="$dirPath/*.wav"
if($?ESPEAK_SINGLE) then
    if($ESPEAK_SINGLE == 1) then
        set dirPath=$basePath:q
        set wavFiles=$dirPath:q/$tailPath:q.wav
    endif
endif

mkdir -p $dirPath:q

csh ./espeak.csh "$file" "$dirPath/$tailPath.wav" 
if($? == 0) then
    set idx=1
    foreach i ($wavFiles)
        #ffmpeg -i "$i" -acodec wmav2 -ab 32000 "$i:r.wma"
        #set ofile="$i:r.ogg"
        #ffmpeg -i "$i" -acodec libvorbis -ab 32000 "$ofile"
        set ofile="$i:r.mp3"
        [ which avconv ] && avconv -y -i "$i" -ab 32000 "$ofile" || ffmpeg -y -i "$i" -acodec libmp3lame -ab 32000 "$ofile"
        #set ofile="$i:r.mp4"
        #ffmpeg -i "$i" -acodec aac -ab 32000 "$ofile"
        if($? == 0) then
            rm "$i"
            id3v2 -a "$tailPath" -A "$out" -t "$ofile:t" -T "$idx" "$ofile"
            @ idx ++
        endif
    end
endif
