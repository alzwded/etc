#!/bin/csh
du -s *.mp3 | cut -f1 | perl -e '$s = 0; while(<>) { $s += $_;} $s=$s/1024; printf "%.0fM\n", $s;'
