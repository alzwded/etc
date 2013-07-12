#!/bin/csh
foreach i ($*:q)
    ffmpeg -i $i:q -y -qscale 7 -acodec libmp3lame -pix_fmt yuv420p -s qvga $i:r:q.mp4
end
