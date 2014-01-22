#!/bin/bash

idx=0
function pad {
    idx=`perl -e 'my $i = "" . '$i'; while(length $i < 3) { $i = "0".$i; }; print $i;'`
}

for (( i=0 ; $i < 115 ; i=`expr $i + 1` )) ; do
    pad $i
    echo page $idx
    convert -density 440 'cpsotr..pdf['$i']' cpsotr-$idx.jpg
done
