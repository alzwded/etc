#!/bin/csh
youtube-dl -f 18 -x --audio-format mp3 --embed-thumbnail --add-metadata --metadata-from-title '%(artist)s: %(title)s' -o '%(title)s.$(ext)s' $*:q
