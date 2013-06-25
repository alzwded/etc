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
