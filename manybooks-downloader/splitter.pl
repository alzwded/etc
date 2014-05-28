#!/usr/bin/perl -w

use File::Path qw/make_path/;
use File::Copy qw/move/;

die 'splitter [-d target] file1 file2 ...' unless(scalar @ARGV > 0);

my $targetDir = ".";
if($ARGV[0] eq '-d') {
    $targetDir = $ARGV[1] or die;
    shift @ARGV;
    shift @ARGV;
}

my @files = @ARGV;
my %dests = map { $_ => "$targetDir/".&fromChar(lc(substr $_, 0, 1))."/" ; } @files;

foreach (keys %dests) {
    my $file = $_;
    my $dest = $dests{$file};
    $dest =~ s#/ /#/Misc/#g;
    print "$file => $dest\n";
    if(not -d $dest) {
        make_path($dest) == 1 or die;
    }
    move($file, $dest) or die;
}

sub fromChar {
    my ($char) = @_;
    if($char =~ m/[a-z]/) {
        return $char;
    }
    return "Misc";
}
