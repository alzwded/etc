#!/bin/csh

if($#argv != 1) then
    echo "usage: $0 on|off|svga|mirror"
    exit 1
endif

set prefix=$0:h:q
if($prefix != "") then
    set prefix=$prefix:q/
endif

switch($1)
case "on":
case "dual":
case "1":
    ${prefix}xm-on.sh
    breaksw
case "off":
case "single":
case "0":
    ${prefix}xm-off.sh
    breaksw
case "svga":
case "800x600":
case "8":
    ${prefix}xm-800x600.sh
    breaksw
case "mirror":
case "2":
    ${prefix}xm-mirror.sh
    breaksw
case "-h":
case "--help":
case "--usage":
case "help":
    echo "usage: $0 on|off|svga|mirror"
    echo "alternative forms:"
    \echo -e "\toff, 0, single"
    \echo -e "\ton, 1, dual"
    \echo -e "\tsvga, 8, 800x600"
    \echo -e "\tmirror, 2"
    breaksw
default:
    echo "usage: $0 on|off|svga|mirror"
    exit 1
    breaksw
endsw
