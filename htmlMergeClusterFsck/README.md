Some backstory
==============

this is the trouble I had to go through to merge some split/indexed html book into a single rendered document.

The premise
-----------

Let there be some documentation that is only available as an indexed book (i.e. it has next/previous/up/contents navigation links). I wanted to have a single html/pdf thing containing everything.

The workflow
------------

1. downloaded said html book (wget with a regex)
2. run a perl script that searches for anchors pointing to local files
3. dump them in the main index as DIVs
4. add some javascript that extracts only the main content from the page and tacks them on at the end
5. in chrome, hide stuff like header, footer, sidebars etc with css
6. print as pdf
7. html2pdf to get an html file that is clear of fluff
