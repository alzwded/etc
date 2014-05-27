#!/bin/csh

onintr cleanup

set i=1
set maxi=19
set pageTmp=""
set linksTmp=""

while($i <= $maxi)
    set link="http://manybooks.net/categories/PUL/$i"
    echo at link $link
    set pageTmp=`mktemp`
    echo "  downloading page..."
    curl $link > $pageTmp
    set linksTmp=`mktemp`
    echo "  extracting links..."
    perl link-extractor.pl $pageTmp > $linksTmp 
    if( $? == 0 ) then
        rm $pageTmp
        set pageTmp=""

        echo "  downloading books..."
        perl downloader.pl $linksTmp

        rm $linksTmp
        set linksTmp=""
    else
        echo "failed to extract any links; refer to $pageTmp"
        exit 1
    endif

# need to POST to /_scripts/download-ebook.php
#   1:epub:.epub:epub
# then, download this:
# http://manybooks.net/_scripts/send.php?tid=jamesonmalother08wreckers_of_the_star_partrol&book=1:epub:.epub:epub
# http://manybooks.net/titles/jamesonmalother08wreckers_of_the_star_partrol.html
    @ i++
end

cleanup:
if( $linksTmp != "" ) then
    rm $linksTmp
endif
if( $pageTmp != "" ) then
    rm $pageTmp
endif
