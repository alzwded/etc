index.pl
========

Awesome script that runs on lighttpd/fastcgi that allows browsing your computer through http, thus allowing me to access my files through any device that supports a browser/http. I don't know what you use it for.

This cgi-script has no reagard for security. The webserver user must be in the "access all filesystems" group on your system to access everything.

It (hopefully) supports partial and resume downloads and downloads in blocks.

TODO
====

* refractor the code because it's a bit of a clusterfuck in the `binary_page` sub department.
* sort directory entries in a nice way (directories first, case insensitive, alphabetically)
* switch to monospaced font
* send header's in directory listing html page response as well (don't let people guess)
* give or take, ability to escape out of the current directory through [..] without needing to type in index.pl?p=/ in the address bar
* OMG it just hit me, please use fseek when processing Range: requests instead of reading in the entire file up to that point FFS