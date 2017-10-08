#!/bin/csh

while(1)
    set a=`ls *.m4a | wc -l`
    set b=`ls *.mp3 | wc -l`
    set c=`expr $b \* 100 / $a`
    echo `date '+%T'`: $b/$a=$c% \(`./size.csh`\)
    sensors
    sleep 30
end
