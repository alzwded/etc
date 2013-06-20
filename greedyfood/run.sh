#!/bin/bash
for i in test*food
do
    echo $i
    cat $i | ./food.pl
    echo --------------
done
