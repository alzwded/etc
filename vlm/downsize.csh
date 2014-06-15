#!/bin/csh

# apparently vlm doesn't actually deliver on transcoding,
# or it simply doesn't work for me
# so first transcode the video with this to something incredibly tiny and be done with it

if( $#argv != 1 ) then
    echo usage: $0 video
    exit 255
endif

avconv -i $1:q -vcodec libtheora -acodec libvorbis -b 400000 -ab 128000 -s qcif $1:r:q.ogv
