#!/bin/bash

[ $# -lt 2 ] && exit 3

FILE="$1"
shift

INPUTS=""

DIR=`mktemp -d TOPDF-XXXXXX`
for i in $@
do
    convert "$i" "$DIR/$i.pdf"
done

pdftk "$DIR"/*pdf cat output "$FILE" 

[ $? -eq 0 ] && rm -rf "$DIR"
