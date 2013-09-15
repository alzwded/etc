#!/bin/csh

if($#argv < 2) then
    \echo "usage: $0 filename output"
    exit 1
endif

set file=$1
shift

set wavFile=$1
shift

set lineLength=50 # lines < cause end-of-statement
set speed=200 # words per minutes
set minutes=20 # split wav after minutes

# 1 capital letters click
# 2 speak 'capitals'
# >3 special emphasis, the higher the more emphasis
set capitalLetters=1

espeak -l $lineLength -s $speed -k $capitalLetters --split=$minutes -f $file -w $wavFile

exit $?
