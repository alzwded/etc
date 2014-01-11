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

./espeak.csh "$file" "$out/$out.wav" && ./encode.csh "$out/$out"_*wav && rm "$out"/*wav
