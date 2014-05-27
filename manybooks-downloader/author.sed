# find the "Creator(s):" line, grab the next one, get rid of redundant author name, print
/Creator(s):/ {
    n
    s/\(.*\) (\1)/\1/
    p
}
