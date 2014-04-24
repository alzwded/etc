#!/usr/bin/perl -w

my $doc = <<EOT
Usage: spotdups.pl file1 file2 file3 ...

spotdups.pl finds duplicate lines in each of the files passed as arguments
and prints the duplications in the following format:

    output: | dup-list ;
    dup-list: dup-list dup | dup ;
    dup: matching-string NL file1 NL file2 NL NL ;
    NL: "\n" ;

This script was used to find the many duplicates in some apt sources.list.d
EOT
;

my %entries = ();
my $dups = "";

foreach (@ARGV) {
    my $file = $_;
    open A, $file;
    while(<A>) {
        chomp;
        if($_ eq "") { next; }
        if(defined($entries{$_})) {
            $dups .= "$_\n" . $entries{$_} . "\n" . $file . "\n\n";
        } else {
            $entries{$_} = $file;
        }
    }
    close A;
}

print $dups;
