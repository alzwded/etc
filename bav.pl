#!/usr/bin/perl -w

use Cwd;

my @files = @ARGV;

die "usage: bav.pl files" unless scalar @files > 0;

my $basename = cwd();
if(scalar @files == 1) {
    $basename = $files[0];
}

$basename =~ s#^.*/([^/]*)\.?.*$#$1#;
print "$basename\n";

my $date = `date +%Y%m%d%H%M%S`;
chomp $date;
my $cmd = "tar cjf '$basename-$date.tbz' ";
foreach (@files) {
    $cmd .= "'$_' ";
}

system("$cmd"); # don't fail because utime fails on windows

open A, ">", "$basename-$date.msg" or die "failed to open $basename-$date.msg";

print("> ");
while(<STDIN>) {
    chomp;
    my $line = $_;
    if($line =~ m/^$/) {
        last;
    } else {
        print A "$line\n";
        print("> ");
    }
}

close A;
print "\n";
