#!/bin/bash

a=`\du $@ | cut -f 1`
c=`echo "$a" | wc -l`

pluses=""
for (( ; $c > 1 ; c=`expr $c - 1` )) ; do
    pluses="$pluses +"
done

res=`echo 2k $a $pluses 1024 / p | dc`
echo "$res"M
