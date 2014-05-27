#!/bin/csh

foreach i (*.epub)
    # pipe epub info into title.sed, get rid of whitespace
    set title=`einfo $i:q | sed -n -f title.sed | cut -c 4-`
    # pipe epub info into title.sed, get rid of whitespace
    set author=`einfo $i:q | sed -n -f author.sed | cut -c 9-`

    set newTitle="$author - $title.epub"
    if($i:q != $newTitle:q) then
        mv $i:q $newTitle:q
    endif
end
