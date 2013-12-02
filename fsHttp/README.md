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
* ~~OMG it just hit me, please use fseek when processing Range: requests instead of reading in the entire file up to that point FFS~~
* investigate which line ending should be used. Are posix `\n` okay or are `\r\n`'s really required?

USAGE
=====

This runs on linux (because of the hard-coded dependencies on `stat` and `file` and posix paths and possibly other things).

It was only tested on lighttpd/fastcgi and thus expects the server to communicate with in through environment variables.

You need to set up your lighttpd server to use mod_cgi and to set up the perl hook for .pl files.

It's useful to put the `www-data` user in the UNIX group that is allowed to access external mounts or whatnot. Or, just add it in the group that's supposed to be allowed to access whatever you want it to access.

Then, point a browser or curl or whatever to your machine running the server and wham-o.

For example, if you have the default setup and you put `index.pl` in /var/www, you can access it through `http://localhost/` or `http://machinename/` or `http://<ip address>/` or something like that.

It was tested to stream video to android and it appears to work, albeit only sequencially (no seeking for whatever reason). This will require more investigation.

`wget` might not work on it. There is a known issue where `wget` fails to do anything and enters a loop if it receives the `206 Partial Content` response. It is not supposed to do this, and it's a bug. Maybe by the time you're reading this that issue was resolved and it now works.

`curl` works fine.

Have fun with it! Feel free to use the knowledge in this script for your own needs in any way imaginable!
