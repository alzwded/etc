#!/bin/csh

if($#argv != 1) then
    echo "usage: $0 on|off|svga|mirror"
    exit 1
endif

switch($1)
case "on":
case "dual":
case "1":
    xrandr -display :0 --output LVDS1 --auto --output VGA1 --auto --left-of LVDS1
    breaksw
case "off":
case "single":
case "0":
    xrandr --output LVDS1 --auto --output VGA1 --off
    breaksw
case "svga":
case "800x600":
case "8":
    xrandr -display :0 --output LVDS1 --auto --primary --output VGA1 --mode 800x600 --left-of LVDS1
    breaksw
case "mirror":
case "2":
    xrandr -display :0 --output LVDS1 --mode 800x600 --output VGA1 --same-as LVDS1
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
