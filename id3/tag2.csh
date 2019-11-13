#!/bin/csh

if(! $?ALBUM) then
    echo ALBUM env var not set
    exit 2
endif

if(! $?ARTIST) then
    echo ARTIST env var not set
    exit 2
endif

set idx=1
foreach i (*.mp3)
    set newFile="$i:r:q-i.mp3"

    set Album=$ALBUM:q
    set Artist=$ARTIST:q

    if($?STRIP) then
        set Title=`echo $i:t:r:q | sed -e "s/^$STRIP//"`
    else
        set Title=$i:r:t:q
    endif
    set Track=$idx:q

    id3v2 --artist $Artist:q --album $Album:q --song $Title:q --track $Track:q $i:q

    if("$Track:q" != "$idx:q") then
        @ idx++
    else if({ test "$Track:q" -ge "$idx:q" >& /dev/null }) then
        @ idx = $Track + 1
    else
        @ idx++
    endif
end
