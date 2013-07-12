#!/bin/csh
foreach i ($*:q)
    ffmpeg -i $i:q -y -qscale 5 -acodec libmp3lame -pix_fmt yuv420p -s qvga $i:r:q.mp4
end
