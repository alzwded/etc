#!/bin/csh

set prefix=/home/jakkal/eBooks/tts
set file=/data/Bueckher-GenericMusic1-x2.wav
set bgmVol=0.13
set voiceVol=0.9

foreach i ($argv[*])
    #sox -m -v $bgmVol "$prefix/$file" -v $voiceVol "$i" -t wav - | ffmpeg -loglevel quiet -i pipe:0 -ab 32000 -acodec wmav2 "$i:r.wma" 
    ffmpeg -loglevel quiet -i "$i" -ab 32000 -acodec wmav2 "$i:r.wma"
end

exit $?
