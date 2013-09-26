Conway's Game of Life in 15 minutes
===================================

This is an implementation of Conway's Game of Life done in less than 15 minutes.

This was the second attempt.

Tactics:
1.  Implement a quick sparse matrix class of booleans. Quickest way was with a set of pairs representing the _true_ values.
2.  Use two SparseMatrix's that alternate between odd and even generations. This required a clear() method and a generation counter.
3.  Implement the iterative algorithm. Pretty straight forward with the SparseMatrix. Since we don't know any bounds, the matrix needs to keep track of them. This is done in the set() method.
4.  Implement printing. This is easy.
5.  Done.

Bugs
====

I'm pretty sure it reads some 0/random values by accident because I don't check that scanf actually read what it was supposed to. Oups.

Also, I didn't reset the bounds (oups!) This means min/maxes were init with 0, 0 in the even generations...

Missing return statement in main (oups)

Working version
===============

Bugs were fixed in gof2_fixed.cpp, but this took longer than 15 minutes.

Also, this version has comments.

Compiling
=========

```sh
g++ -O3 --std=gnu++11 gof2.cpp && ./a.out < test > log & sleep 10 && killall -9 a.out
```

or
```sh
g++ -O3 --std=gnu++11 gof2_fixed.cpp && a.out < test > log & sleep 10 && killall -9 a.out
```

If you're curious about version one
===================================

It sucked. It took half an hour and it wasn't working. It involved a lot of maps, structs, methods, stuff from stl's algorithm and lambdas. Don't do that if you want to finish fast. Keep it simple, stupid.
