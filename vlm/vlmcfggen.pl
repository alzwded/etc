#!/usr/bin/perl

die 'not enough parameters' unless scalar @ARGV > 1;

my $cfgName = shift @ARGV;

die 'cfg file name should start with a dash (-) (safety reasons)' unless ((substr $cfgName,0,1) eq "-");

$cfgName = substr $cfgName, 1;

open A, ">$cfgName";

print A "vlm\n";

my $idx = 0;
foreach $file (@ARGV) {
    print A "new T$idx vod enabled\n";
    print A "setup T$idx input \"$file\"\n";
    #print A "setup T$idx output #transcode{vcodec=h264,vb=400,width=800}\n";
    ++$idx;
}

close A;
