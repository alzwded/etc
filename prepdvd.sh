#!/bin/sh

if [[ $# < 1 ]] ; then
    echo "Usage: $0 song1 song2 ..."
    exit 2
fi

for i in "$@" ; do
    ffmpeg -y -f lavfi -i color=size=720x480:rate=25:color=black -i "$i" -c:a pcm_s16le -shortest -c:v libxvid "${i%.*}.mov"
done

echo "----------------------------------------"
echo "Use DeVeDe to create a Video DVD project"
echo "(add a menu with a playall option...)"
