#!/bin/sh

IF=/dev/zero

if [[ -z $COUNT ]] ; then
    COUNT=1000000
fi

if [[ $# -gt 0 && $1 -eq 'random' ]] ; then
    echo creating randomized file
    IF=/dev/random
fi

[[ -e largefile ]] && echo 'file exists, not overwriting' && exit 1
echo "writing $COUNT records of 512 bytes"
dd if=$IF of=largefile bs=512 count=$COUNT
