#!/bin/sh
l1w=110
l2w=65
c1=0xFF410D
c2=0xD6D25A
./lineGenerator test.png 800 800 $l1w $l2w $c1 $c2
display test.png
