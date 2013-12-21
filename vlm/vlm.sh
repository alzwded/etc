#!/bin/bash

# usage:
# $ vlmcfggen.pl cfg file1.mp4 file2.mp4
# $ vlm.sh
# > vlm
# > load cfg
#
# client:
# $ vlc rtsp://host:5554/T0
# $ vlc rtsp://host:5554/T1
#
# TODO
# set up transcoding to something that fits in 800x600, ~400kbps or something like that

cvlc -I cli --rtsp-host 0.0.0.0 --rtsp-port 5554 --ttl 2
