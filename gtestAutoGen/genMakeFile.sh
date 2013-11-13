#!/bin/bash

cat Makefile.template > _Makefile

for i in $* ; do
    j=$(echo "$i" | sed -e 's/_unittest$//g')
    cat Makefile.atom.template | sed -e "s/\%/$j/g" >> _Makefile
done
