#!/bin/bash

codec=mp3
br=96000
extension=mp3
avconv_bin=avconv

for i in */*aac ;
do
    echo "converting $i"
    [ -e "${i:0: -4}.$extension" ] \
        && echo "$i already converted, cowerdly skipping" \
        || $avconv_bin -i "$i" -acodec $codec -ab $br "${i:0: -4}.$extension"
done
