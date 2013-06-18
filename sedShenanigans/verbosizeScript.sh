#!/bin/bash

# make a script print each line it's about to execute
# heredocs don't work :-??

[ $# -ne 1 ] && exit 2

sed -e '
1 a \
 # this proves why bash is uh... awesome? \
 # lines begining with spaces are meta \
 # prints a non-meta line from this file and executes it \
 sed -e "1 d ; /^  *[^ ].*/d" $0 | \
     while read line ; do \\\
         [ "$line" != "" ] \\\
             && echo "# $line" \\\
             && eval "$line"\\\
         || echo "" ; \\\
     done \
 exit 0 # exit main program' "$1" 
