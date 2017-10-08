#!/bin/csh

[ -f lastindex ] || echo noindex && exit 2
set index=`cat lastindex`
@ index++
foreach i (*.m4a)
    echo $index\: $i:q
    avconv -i $i:q -acodec libmp3lame -q 8 -y $i:r:q.mp3 > /dev/null
    echo ...setting tag
    id3v2 -A 'That Pedal Show' -a 'That Pedal Show' -t $i:r:t:q -T $index $i:r:q.mp3
    @ index++
end
