#!/bin/csh
foreach i ($*:q)
    #ffmpeg -i $i:q -y -qscale 7 -acodec libmp3lame -pix_fmt yuv420p -s qvga $i:r:q.mp4
    # note to self -- that went worse. Sigh
    # I think this has something to do with the file size or whatnot
    ffmpeg -i $i:q -y -acodec copy -s qvga "$i:r.qvga.$i:e"
end
