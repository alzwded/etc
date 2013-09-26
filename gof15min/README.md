Conway's Game of Life in 15 minutes
===================================

This is an implementation of Conway's Game of Life done in less than 15 minutes.

This was the second attempt.

Tactics:
1.  Implement a quick sparse matrix class of booleans. Quickest way was with a set of pairs representing the _true_ values.
2.  Use two SparseMatrix's that alternate between odd and even generations. This required a Clear() method and a generation counter.
3.  Implement the iterative algorithm. Pretty straight forward with the SparseMatrix. Since we don't know any bounds, the matrix needs to keep track of them. This is done in the set() method.
4.  Implement printing. This is easy.
5.  Done.

Bugs
====

I'm pretty sure it reads some 0/random values by accident because I don't check that scanf actually read what it was supposed to. Oups.

Also, I didn't reset the bounds (oups!) This means min/maxes were init with 0, 0 in the even generations...

Working version
===============

Bugs were fixed in gof2_fixed.cpp, but this took longer than 15 minutes.
