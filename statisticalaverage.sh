#!/bin/bash

mu() {
    local i
    local N=$#
    local N1=$(( "$N" - 1 ))
    local ops=( 3k )
    for i in "$@" ; do
        ops=( "${ops[@]}" "${i//-/_}" )
    done
    for i in $( seq $N1 ) ; do
        ops=( "${ops[@]}" + )
    done
    ops=( "${ops[@]}" "$N" / p )
    echo "${ops[@]}" | dc
}

sigma2() {
    local i
    local m="${1//-/_}"
    shift > /dev/null
    local devs=()
    for i in "$@" ; do
        devs=( "${devs[@]}" "$( echo 3k "${i//-/_/}" "$m" - d \* p | dc )" )
    done
    mu "${devs[@]}"
}

sigma() {
    local s2
    s2="$( sigma2 "$@" )"
    echo 3k "${s2//-/_}" v p | dc
}

NUMBERS=( "$@" )
MU="$( mu "${NUMBERS[@]}" )"
SIGMA="$( sigma "$MU" "${NUMBERS[@]}" )"
echo "$MU" "$SIGMA"
