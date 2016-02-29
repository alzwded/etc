#!/bin/sh
cat "$0" | sed -e '8,$d;1,2d'
sed -e '/\(patt\)ern/ {s//new\1/}' <<EOF
pattern
funky pattern
funky pattern town
EOF

exit
will print:
    newpatt
    funky newpatt
    funky newpatt town
