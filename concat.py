#!/usr/bin/python

import sys
from glob import glob 
from getopt import getopt, GetoptError

def usage():
    sys.stderr.write("Usage: %s [-o outfile] input1 input2...\n" % sys.argv[0])
    sys.exit(1)

try:
    optlist, args = getopt(sys.argv[1:], 'o:p')
except GetoptError as err:
    print(str(err))
    usage()

if len(args) <= 0:
    usage()

outfile = ''
g = sys.stdout
concatParagraphs = False
for o, a in optlist:
    if o == '-o':
        sys.stderr.write("Writing to %s\n" % a)
        outfile = a
        g = open(outfile, "w")
    if o == '-p':
        sys.stderr.write("Merging paragraphs\n")
        concatParagraphs = True


for j in args:
    sys.stderr.write(repr(glob(j)))
    for i in glob(j):
        if i == outfile: continue
        # you know, I could refactor this out into something that looks like
        # lambda calculus, but python won't let me without going OOP on it.
        # So I'll probably refactor it in another language, to keep *this*
        # code pythonic.
        prevLine = ""
        with open(i, "r") as f:
            for line in f:
                if concatParagraphs:
                    if len(line.rstrip()) == 0:
                        g.write("\n%s" % line)
                        prevLine = line
                    else:
                        g.write("%s" % line.rstrip())
                        prevLine = line.rstrip()
                else:
                    g.write("%s" % line)

if g != sys.stdout: g.close()
