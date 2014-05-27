#!/bin/csh

set i=1
set maxi=19

while($i <= $maxi)
    set link="http://manybooks.net/categories/PUL/$i"
    set pageTmp=`mktemp`
    curl $link > $pageTmp
    set linksTmp=`mktemp`
    perl link-extractor.pl $pageTmp > $linksTmp
    rm $pageTmp

    perl downloader.pl $linksTmp

# need to POST to /_scripts/download-ebook.php
#   1:epub:.epub:epub
# then, download this:
# http://manybooks.net/_scripts/send.php?tid=jamesonmalother08wreckers_of_the_star_partrol&book=1:epub:.epub:epub
# http://manybooks.net/titles/jamesonmalother08wreckers_of_the_star_partrol.html
    @ i++
end
