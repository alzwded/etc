#!/usr/bin/perl -w
 use File::MimeInfo;

open A, "<playlist.m3u";
open B, ">playlist-2.m3u";

while(<A>) {
    chomp;
    my $file = $_;
    print B "$file\n" if (mimetype($file) =~ m|^audio/.*$|);
}

close A;
close B;
