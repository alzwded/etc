#!/bin/csh

foreach i ($*:q)
    echo 'Code Listing for '$i':'
    \cat -n $i
    echo
end
