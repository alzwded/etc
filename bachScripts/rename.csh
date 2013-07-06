#!/bin/csh

if( $#argv < 1) then
    echo "no directory"
    exit 2
endif

set dir="$1"

cd $dir:q/*/

echo "[renames]" > rename.log

set idx=1000
foreach i (*mp3)
    echo ". $i:q -"'>'" $idx:qq.mp3"
    echo "$i:q -"'>'" $idx:qq.mp3" >> rename.log
    mv $i:q $idx.mp3
    @ idx ++
end
