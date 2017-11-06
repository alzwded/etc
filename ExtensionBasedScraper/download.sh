#!/bin/sh

if [ $# -lt 2 ] ; then
    echo usage: $0 fileToScrape extension
    exit 2
fi

mydir=`dirname $0`
mydir=`readlink -f $mydir`

for i in $($mydir/scraper.sh "$1" "$2") ; do
    links=`mktemp`
    echo getting temp page \"$i\" into \"$links\"
    curl "$i" > "$links"
    words=$($mydir/scraper.sh "$links" "$2")
    unlink "$links"
    for i in $words ; do
        tail=`echo "$i" | sed -E 's/^(.*?)\/([^\/]*)$/\2/'` 
        stupidSed='s/\//\\\//g'
        tail=`echo "$tail" | sed -E "$stupidSed"`
        if [ -z $tail ] ; then
            tail="$i"
        fi
        echo getting \""$i"\" into \""$tail"\"
        if $(curl "$i" > ./"$tail") ; then
            break
        fi
    done
done
