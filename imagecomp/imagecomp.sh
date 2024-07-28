#!/bin/bash

#convert -list dither
#[ -f pallette.xpm ] || convert xc:#000000 xc:#ff00ff xc:#00ffff xc:#ffffff +append pallette.xpm
[ -f pallette6.xpm ] || convert xc:#000000 xc:#ff0000 xc:#ffff00 xc:#00ffff xc:#0000ff xc:#ffffff +append pallette6.xpm

diff -u <(identify -verbose $1) <(identify -verbose $2)

reduce() {
    #convert "$1" -resize 32x32\! -posterize 2 -colors 4 -remap pallette.xpm XPM:- | tail -n 33
    convert "$1" -resize 32x32\! -posterize 2 -colors 6 -remap pallette6.xpm XPM:- | tail -n 33
    #convert "$1" -resize 32x32\! -dither FloydSteinberg -colors 4 -remap pallette.xpm XPM:- | tail -n 33
    #convert "$1" -resize 32x32\! -dither FloydSteinberg -colors 6 -remap pallette6.xpm XPM:- | tail -n 33
}

IFS='
'
A=( $( reduce "$1" ) )
B=( $( reduce "$2" ) )
for (( i=0 ; i < 32 ; ++i )) ; do
    echo "${A[$i]}" "${B[$i]}"
done


#convert $1 -resize 32x32\! -dither FloydSteinberg -colors 4 -remap pallette.xpm sixel:-
#convert $2 -resize 32x32\! -dither FloydSteinberg -colors 4 -remap pallette.xpm sixel:-
