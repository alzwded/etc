#!/usr/bin/perl

die 'not enough parameters' unless scalar @ARGV > 1;

my $cfgName = shift @ARGV;

open A, ">$cfgName";

my $idx = 0;
foreach $file (@ARGV) {
    print A "new T$idx vod enabled\n";
    print A "setup T$idx input \"$file\"\n";
    ++$idx;
}

close A;
