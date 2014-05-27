#!/usr/bin/env perl -w

my ($file) = @ARGV;

open A, "<$file";
my $ret = 1;

while(<A>) {
    chomp;
    my $line = $_;

    if($line =~ /<a href="([^"]*)">/) {
        my $potential = $1;
        if($potential =~ m#^/titles/(.+)\.html#) {
            my $title = $1;
            #print "http://manybooks.net/_scripts/send.php?tid=$title&book=1:ebup:.epub:epub\n";
            print $title."\n";
            $ret = 0;
        }
    }
}

close A;

exit $ret;
