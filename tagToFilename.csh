#!/bin/csh

if($1:q == -h) then
    echo if it looks okay, run:
    echo foreach i '("`'$0' -0`")'
    echo '    $i'
    echo end
    echo which won\'t work
    echo 'setenv DOIT / export DOIT=1 to execute'
endif

foreach i ( $*:q )
    set TRACK=`id3v2 --list $i:q | grep '^TRCK [^:]* \([0-9]*\)' | sed -e 's/TRCK [^:]*: \([0-9]\{1,\}\).*/\1/'`
    set TRACK=`printf "%02d" $TRACK:q`
    set TITLE=`id3v2 --list $i:q | grep '^TIT2' | sed -e 's/TIT2 [^:]*: \(.*\)$/\1/'`
    set TPOS=`id3v2 --list $i:q | grep '^TPOS [^:]* \([0-9]*\)' | sed -e 's/TPOS [^:]*: \([0-9]\{1,\}\).*/\1/'`
    if ( $%TPOS > 0 ) then
        set TPOS=`printf "%02d" $TPOS:q`
        set TRACK="$TPOS:q-$TRACK:q"
    endif
    set DEST="$TRACK:q. ${TITLE:gs,/,_,:q}.${i:e:q}"
    if( ! $?DOIT ) then
        echo "mv '$i:q' '$DEST'"
        echo 'setenv DOIT / export DOIT=1 to execute'
    else
        mv $i:q $DEST:q
    endif
end

