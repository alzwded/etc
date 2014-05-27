#!/bin/csh

foreach i (*.epub)
    set title=`einfo $i:q | sed -n -f title.sed | cut -c 4-`
    set author=`einfo $i:q | sed -n -f author.sed | cut -c 9-`

    mv $i:q "$author - $title.epub"
end
