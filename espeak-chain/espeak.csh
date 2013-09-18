#!/bin/csh

if($#argv < 2) then
    \echo "usage: $0 filename output"
    exit 1
endif

set file="$1"
shift

set wavFile="$1"
shift

set lineLength=50 # lines < cause end-of-statement
set speed=200 # words per minutes
set minutes=20 # split wav after minutes
set pitch=90 # 0-99
#set voice=lancashire
#set voice=en-scottish
#set voice=en-swedish

set pitch=80
set voice=english-mb-en1
set speed=200

# 1 capital letters click
# 2 speak 'capitals'
# >3 special emphasis, the higher the more emphasis
set capitalLetters=1

espeak -l $lineLength -s $speed -k $capitalLetters --split=$minutes -p $pitch -v $voice -f $file:q -w $wavFile:q

exit $?
