#!/usr/bin/perl -w

use strict;
use warnings;

my $SIZE = 64;
my $first = 33;
my $last = 195;
my @a = 0..255;

TruncateRange($first, $last);

foreach my $i (@a) {
    printf("%d ", $i);
}
printf("\n");

sub TruncateRange {
    my ($first, $last) = @_;

    my $ofirst = $first;
    my $p = $last;
    my $totalLen = 256;
    my $len = $totalLen - $last;
    my $copied = 0;

    while($copied < $len) {
        my $base1 = $first & 63;
        my $base2 = $p & 63;

        my $d1 = (($SIZE - ($first - $base1)) < $len - $copied)
            ? $SIZE - ($first - $base1)
            : $len - $copied;
        my $d2 = (($SIZE - ($p - $base2)) < $len - $copied)
            ? $SIZE - ($first - $base1)
            : $len - $copied;
        my $d = ($d1 < $d2) ? $d1 : $d2;
        for(my $i = 0; $i < $d; ++$i) {
            $a[$first + $i] = $a[$p + $i];
        }
        $first += $d;
        $p += $d;
        $copied += $d;
    }

    splice @a, $ofirst + $len;
}
