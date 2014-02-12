#!/usr/bin/tcsh

foreach i ($*:q)
    echo PROCESSING $i
    if( $i:q == $i:r:q ) then
        mv $i:q $i:r:q.zip
    endif
    # mkdir -p $i:r:q
    # mv $i:r:q.zip $i:r:q
    # file-roller -h $i:r:q/$i:r:q.zip || echo FAILED $i
    file-roller -h $i:r:q.zip || echo FAILED $i
end
