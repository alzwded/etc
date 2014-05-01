#!/bin/csh

if($# > 0) then
    set filter=$1
else
    set filter=wma
endif

set total=`find . -name \*.aac | wc -l`

while(1)
    set current=`find . -name \*.$filter | wc -l`
    echo $current / $total
    sleep 30
end
