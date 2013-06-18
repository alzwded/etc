#!/bin/bash
 # this proves why bash is uh... awesome?
 # lines begining with spaces are meta
 # prints a non-meta line from this file and executes it
 sed -e '1 d ; /^  *[^ ].*/d' $0 | \
     while read line ; do \
         [ "$line" != '' ] \
             && echo "# $line" \
             && eval "$line"\
         || echo '' ; \
     done
 exit 0 # exit main program
echo ${a:-good}
echo ${a:=a/b/c.d.e}
echo ${a:1}
echo ${a:1:1}

echo ${a%.*} # extensionless
echo ${a%%.*} # more dramatic
echo ${a##*/} # tail
echo ${a#*/} # :-??
echo ${a%/*} # dir

echo ${a//\//\\\\}
echo ${a/\//\\\\}

b=AAaa
echo ${b,,*}
echo ${b,}
echo ${b^^*}
b="hello there!"
echo ${b^}
