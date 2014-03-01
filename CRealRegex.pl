#!/usr/bin/perl -w

# believe it or not, you are required to write this to get your
# bachelor's at my faculty...

# TODO actually look up the latest C standard on what exactly is allowed
# or not, this was written off the top of my head

my $regex = '^[-+]?([0-9]+\.?|[0-9]*\.[0-9]+)([f]|[l])?$';
my $s = <STDIN>;
chomp $s;
print "yes\n" if $s =~ m/$regex/;
print "no\n" if $s !~ m/$regex/;
