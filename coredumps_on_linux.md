How to enable core dumps on Rocky (maybe Ubuntu as well):

- set the coredump ulimit to unlimited, e.g. in tcsh `limit coredumpsize unlimited`
- make it findable a la openbsd `sudo sysctl kernel.core_pattern=%e.core` ; don't ask me what the default is, but it goes somewhere in `/var/lib/systemd/coredump` which is unfindable
- voila, coredumps!
