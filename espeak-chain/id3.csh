#!/bin/csh
set echo

if($#argv != 1) then
    echo "id3 foldername"
    exit 1
endif

set fldr="$1"
set extns="mp3"
set idx=1

foreach i ("$fldr"/*$extns)
    id3v2 -a "$fldr" -A "$fldr" -t "$fldr"_"$idx" "$i"
    @ idx ++
end
