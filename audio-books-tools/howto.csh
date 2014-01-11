#!/bin/csh

ffmpeg -i myfile.mp3 -ac 1 -ar 22050 myfile.wav
sox myfile.wav myfile.wav out.wav
