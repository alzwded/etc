#!/bin/csh

foreach i (*.flv *.webm)
    echo '========================================================================'
    echo processing $i
    echo '========================================================================'
    if( -e $i:r:q.mp4 ) then
        echo '========================================================================'
        echo cowardly refusing to overwrite an existing file
        echo '========================================================================'
        continue
    endif
    if( { avconv -i $i:q -acodec libmp3lame -vcodec libx264 -ab 96000 $i:r:q.mp4 } ) then
        echo '========================================================================'
        [ -e $i:r:q.mp4 ] \
            && [ 10 -lt `stat -c "%s" $i:r:q.mp4` ] \
            && echo removing source && rm $i:q \
            || echo maybe conversion of $i was not successful?
        echo '========================================================================'
    else
        echo '========================================================================'
        echo convertion of $i was not successful
        echo '========================================================================'
    endif
end
