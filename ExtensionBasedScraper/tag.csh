#!/bin/csh
set echo

if(! $?ALBUM) then
    echo ALBUM env var not set
    exit 2
endif

if(! $?IMAGE) then
    echo IMAGE env var not set
    exit 2
else if(! -f "$IMAGE") then
    echo $IMAGE is not a file
    exit 2
endif

set idx=1
foreach i (*.mp3)
    set newFile="$i:r:q-i.mp3"

    lame -v -h --ti "$IMAGE:q" "$i:q" "$newFile:q" || exit 3

    set Album=`id3v2 -l "$i:q" | grep '^TALB' | sed -E 's/.*: (.*)/\1/'`
    #if($Album:q == "") then
        # always keep my album string...
        set Album=$ALBUM:q
    #endif
    set Artist=`id3v2 -l "$i:q" | grep '^TPE1' | sed -E 's/.*: (.*)/\1/'`
    if("$Artist" == "") then
        set Artist=$ALBUM:q
    endif
    set Title=`id3v2 -l "$i:q" | grep '^TIT2' | sed -E 's/.*: (.*)/\1/'`
    if("$Title" == "") then
        set Title=$i:t:r:q
    endif
    set Track=`id3v2 -l "$i:q" | grep '^TRCK' | sed -E 's/.*: (.*)/\1/'`
    if("$Track" == "") then
        set Track=$idx:q
    endif

    id3v2 -A "$Album:q" -a "$Artist:q" -T "$Track:q" -t "$Title:q" "$newFile:q" || exit 4
    if("$Track:q" != "$idx:q") then
        @ idx++
    else if({ test "$Track:q" -ge "$idx:q" >& /dev/null }) then
        @ idx = $Track + 1
    else
        @ idx++
    endif
end
