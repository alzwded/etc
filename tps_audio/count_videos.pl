#!/usr/bin/perl -w

use warnings;
use strict;
use JSON qw/decode_json/;

die 'no playlist' if(!defined($ARGV[0]));

my $jsonStr = `youtube-dl --flat-playlist -J '$ARGV[0]'`;
my $obj = decode_json $jsonStr;
my $n = 0 + @{$obj->{entries}};

print "$n\n";
