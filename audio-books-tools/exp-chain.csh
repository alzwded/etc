#!/bin/csh

set magicNumber = 12

set hup
onintr cleanup

if($#argv != 2) then
    \echo "$0 txt outputName"
    exit 1
endif

set txt=$1
shift
set out=$1
shift

./espeak.csh "$txt" "$out" &
set failCount=0
sleep 5
while(1 == 1)
    set a="$out"/*wav
    if($#a > 1) then
        @ failCount = 0
        set x=$a[1]
        ffmpeg -i "$x" -acodec wmav2 -ab 32000 "$x:r.wma" && rm $x
    endif
    sleep 10
    @ failCount++
    if($failCount > $magicNumber) then
        if($#a == 1) then
            set x=$a[1]
            ffmpeg -i "$x" -acodec wmav2 -ab 32000 "$x:r.wma" && rm $x
        endif
    endif
end


cleanup:
kill -9 %
