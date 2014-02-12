#!/bin/csh

# relative dirs, we want output relative to this directory
# think of this as a central hub of sorts
set indirs=( ../pxl-bot ../8bp ../PetrodactylSquad )

# clear playlist
truncate -s 0 playlist.m3u
foreach dir ($indirs)
    find $dir:q -type f >> playlist.m3u || ( echo FAIL $dir && exit 1 )
end

if( { perl keepOnlyAudio.pl } ) then
    \rm -f playlist.m3u
    mv playlist-2.m3u chiptunes.m3u
endif
