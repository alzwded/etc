#!/bin/bash

if [[ $# -gt 0 ]] ; then
    a=`\du $@ | cut -f 1`
else
    a=`\du -s | cut -f 1`
fi
c=`echo "$a" | wc -l`

pluses=""
for (( ; $c > 1 ; c=`expr $c - 1` )) ; do
    pluses="$pluses +"
done

res=`echo 2k $a $pluses 1024 / p | dc`
echo "$res"M
