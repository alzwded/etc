#!/bin/sh

# slightly improve readability of bad photocopied books using ImageMagick

for i in $@ ; do
    echo `date +%T` $i : first pass
    # okay, slow but it yields MUCH better results
    # ulterior passes are faster but only slightly improve the result
    convert "$i" -normalize -sharpen 0x5 -scale 1800x1800 "$i"_1
    #convert "$i" -normalize -unsharp 0x5+1+0 -scale 1800x1800 "$i"_1
    echo `date +%T` $i : second pass
    convert "$i"_1 -unsharp 0x3+1+0 "$i"_2
    if [ $?  -eq  0 ] ; then
        echo `date +%T` $i : deleting "$i"_1
        rm "$i"_1
    fi
    echo `date +%T` $i : third pass
    convert "$i"_2 -unsharp 0x3+1+0.3 "$i"_3
    if [ $?  -eq  0 ] ; then
        echo `date +%T` $i : deleting "$i"_2
        rm "$i"_2
    fi
    echo `date +%T` $i : fourth pass
    convert "$i"_3 -unsharp 0x3+1+0.3 "$i"_4
    if [ $?  -eq  0 ] ; then
        echo `date +%T` $i : deleting "$i"_3
        rm "$i"_3
    fi
    echo `date +%T` $i : fifth pass
    convert "$i"_4 -unsharp 0x3+1+0.3 "$i"_5.jpg
    if [ $?  -eq  0 ] ; then
        echo `date +%T` $i : deleting "$i"_4
        rm "$i"_4
    fi
done
