etc
===

repo for keeping test / ugly / experimental / scratch / quick / little-programs type projects that don't go anywhere but might contain reusable code / concepts

minifloat
=========

minifloat class and testbed

representing integers as 8bit floating point numbers

http://en.wikipedia.org/wiki/Minifloat

from the message of commit # 7a96e41dde7be5d6f8758b0bcba22b0f4e5fede0 :
* currently, tests pass, however there might be cases where they don't
* ergo, TODO: write tests where +/- fails before moving on to removing the int-conversion dependency in multiply and divide.
* note for above: test when then exponent difference is greater than what's currently tested. Right now it tests for diff=0,1,2
* note for above: it's intended to cause round-off errors for bigger differences anyway, but it still needs to be tested that it works _as intended_ and not by random values. Mmmmm... IEEE 754 is hard... I fully respect those who implemented this is hardware and libraries and whatnot, they were awesome people

...but meanwhile! (as of 24/06/13)
* tests pass for addition, substraction and meta-operations (I think).
Meta operations are stuff like 1/inf, 1/0, inf+inf etc, no actual math involved
* if tests for int-minifloat-int conversion were actual tests, they too
would pass
* multiply and divide rely on int conversion, which is less than stelar

...however! (as of 25/06/13)
* tests pass for +,-,relational,mult & div (somehow)
* division relies on probing the quotient until it's bigger than the divident, because I could not figure out a better way to account for round-off errors (which on these 8bit floats it's pretty darn horrible). I'm sure there's a better way, but I can't be bothered
* I'm pretty much done here
* if there really is a need for bonus points, I could just finish off implementing the stream operators, and maybe some C-like FILEptr functions that accomplish the same thing. Those would be cool
* tests are awesome

agdqScripts
===========

scripts used to transfer stuff off of archive.org. Also, `sizeAll.sh` which does a sum of multiple `du` outputs.

bachScripts
===========

scripts used to extract archives which contain names in dubious code pages to an ntfs fs that doesn't support said code pages.

clpfcdgen & clpromgen
=====================

generate combinations of bits. These were used to finish a school project that involved writing down said combinations.

espeak-chain
============

I consider these some of my most important scripts. These drive espeak to output audio books just the way I like them.

fsHttp
======

cgi script that runs on lighttpd/fastcgi. Used to access my main computer through any browser to transfer files and/or crudely stream stuff.

gmockAutoGen & gstestAutoGen
============================

wrote these for a code retreat event. They generate Makefiles for a project that uses gmock or gtest or something like that. Whatever.

goStuff
=======

this is where my experiments in golang were supposed to go, but didn't.

gof15min
========

I have implemented Conwell's Game of Life in 15 minutes. However, I got the rules wrong. Oups!

greedyfood
==========

I don't remember. Something in perl.

huffmanGenerateForCPP
=====================

I used this to generate a huffman dictionary from cpp files for a LMS Training Center event.

jakcode
=======

Some form of proto encryption or something like that. This will be most useful for a school project that will be due soon (serendipity)

khathonscripts
==============

scripts used to rip stuff from youtube and convert them to mp4 and rename them and other stuff.

loopingList
===========

I honestly don't remember `:-|`. I think this too was for a LMS Traning Center event.

photoProc
=========

scripts I used once to make a hand laser-scanned book readable.

school-PG
=========

opengl school project done in a couple of hours while learning openGL 2.

schoolPapersAndWhatnot
======================

This folder contains school papers and whatnot

sedShenanigans
==============

useful stuff you can do with sed

stlCWrapper
===========

A project I've started long ago to use stl containers in C. Meh

streamRip
=========

script chain used to rip internet radios and then shuffle the ripped contents. I really love the `shuffle.pl` script.

usefulScripts
=============

define useful
