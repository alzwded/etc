#!/usr/bin/perl -w

open A, "<".$ARGV[0];
while(<A>) {
    my $link = $_;
    if($link =~ m/^URL=(.*)$/) {
        print $1;
        exit 0;
    }
}
exit 255;
