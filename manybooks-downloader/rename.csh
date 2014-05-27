#!/bin/csh

foreach i (*.epub)
    # pipe epub info into title.sed, get rid of whitespace
    set title=`einfo $i:q | sed -n -f title.sed | cut -c 4-`
    # pipe epub info into title.sed, get rid of whitespace, get rid of redundant author name
    set author=`einfo $i:q | sed -n -f author.sed | cut -c 9-| sed -e '/\(.*\) (\1)/ s/(.*)//'`

    mv $i:q "$author - $title.epub"
end
