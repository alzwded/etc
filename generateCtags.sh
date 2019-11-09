#!/bin/sh

# --tag-relative=yes  <-- files in tags are relative to tags file and not to cwd
# --fields: enable everything I know of
# --extra: extra stuff for C++
# --c&c++-kinds: enable everything I know of
ctags -f tags --tag-relative=yes --fields=+a+f+i+k+l+m+n+s+S+z+t --extra=+f+q --c-kinds=+c+d+e+f+g+l+m+n+p+s+t+u+v+x --c++-kinds=+c+d+e+f+g+l+m+n+p+s+t+u+v+x *

echo 'vim usage:
    g]              tselect `.
    :tselect [s]    show a pick list of tag definitions
    :tag [s]        go to first matching tag
    :tn
    :tp'
