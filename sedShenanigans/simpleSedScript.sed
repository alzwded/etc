#!/bin/sed

# if every other line contains ala, replace it with bala
# otherwise, exit

# for some reason the first line is always processed, but the next ones
# need to be read in with ,n'
b start
:again
n
:start
n
s/ala/bala/
t again
Q
