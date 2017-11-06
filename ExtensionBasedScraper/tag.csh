#!/bin/csh

if(! $?ALBUM) then
    echo ALBUM env var not set
    exit 2
endif

if(! $?IMAGE) then
    echo IMAGE env var not set
    exit 2
else if(! -f $IMAGE) then
    echo $IMAGE is not a file
    exit 2
endif

set idx=1
foreach i (*.mp3)
    set newFile=$i:r:q-i.mp3
    lame -v -h --ti $IMAGE:q $i:q $newFile:q
    id3v2 -A $ALBUM:q -a $ALBUM:q -T $idx -t $i:t:r:q $newFile:q
    @ idx++
end
