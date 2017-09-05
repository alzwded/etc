#!/bin/sh

[ $# -eq 0 ] && echo usage: $0 filename [outdir] && exit 2

THISPATH=$(readlink -f .)
INFILE=$(readlink -f "$1")
WAVPATHINSAMEPLACE=
WAVPATH=$(readlink -f ${2:-$(echo "$INFILE" | sed -e 's/\(\.\w\+\)\?$/_tts/')})
echo $INFILE to $WAVPATH
echo $WAVPATH
ESPEAK_CHAIN_PATH=${ESPEAK_CHAIN_PATH:-$(dirname $0)}
echo using chain from $ESPEAK_CHAIN_PATH

export ESPEAK_SINGLE=1

# those scripts are directory-sensitive
cd $ESPEAK_CHAIN_PATH
./mini-chain.csh "$INFILE" "$WAVPATH"
