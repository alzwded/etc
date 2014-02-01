#!/bin/csh

# Sometimes when my internet dies in the middle of an apt-something, the
# source lists get all corrupted and apt won't run anymore.
# Purging them seems to fix this state.
# This script asks you an explicit question if you want to delete those
# files since it is run as root and you can never be too careful with these
# kind of things

set findPath="/var/lib/apt/lists/"

find $findPath:q -type f -exec "echo" "{}" ";"
echo will delete all of the above. Continue'? (type "yes" to continue)'
set x=$<

if( $x == "yes" ) then
	echo purging apt lists...
	find $findPath:q -type f -delete
endif

