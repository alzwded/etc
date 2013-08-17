#!/bin/bash

#for i in "KH-a-thon_The_Kingdom_Hearts_marathon_"* ; do
#    mv "$i" "${i:38}"
#done

#for i in "KH-a-thon - The Kingdom Hearts marathon "* ; do
#    #mv "$i" "${i/KH-a-thon - The Kingdom Hearts marathon \[2 - Kingdom Hearts\] /_}"
#    mv "$i" "${i/KH-a-thon - The Kingdom Hearts marathon /_}"
#done

for i in "KH-a-thon"?"-"?"The"?"Kingdom"?"Hearts"?"marathon"?* ; do
    #mv "$i" "${i/KH-a-thon - The Kingdom Hearts marathon \[2 - Kingdom Hearts\] /_}"
    mv "$i" "${i/KH-a-thon?-?The?Kingdom?Hearts?marathon?/}"
done

for i in *"Part "[0-9]"]"* ; do
    t=`echo "$i" | perl -e 'my $s = <STDIN>; chomp $s; $s =~ s/(.*Part) ([0-9])\]/$1 0$2]/; print $s;'`
    mv "$i" "$t"
done
