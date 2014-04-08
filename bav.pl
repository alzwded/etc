#!/usr/bin/perl -w

use Cwd;

my @files = @ARGV;

die "usage: bav.pl files" unless scalar @files > 0;

if($files[0] eq '-l' && scalar(@files) == 2) {
    my @messages = sort split /\n/, `ls -1 '$files[1],'*'.msg'`;

    foreach (@messages) {
        next if $_ eq '';
        my $file = $_;
        $file =~ m/^$files[1],(.*)\.msg$/;
        my $date = $1;
        $date =~ /([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})/;
        my $nicedate = "$1/$2/$3 $4:$5:$6";
        print "$nicedate:\n";
        system "cat '$file'";
        print(("-" x 72)."\n");

    }

    exit 0;
}

my $basename = cwd();
if(scalar @files == 1) {
    $basename = $files[0];
}

$basename =~ s#^.*/([^/]*)\.?.*$#$1#;

my $date = `date +%Y%m%d%H%M%S`;
chomp $date;
my $cmd = "tar cjf '$basename,$date.tbz' ";
foreach (@files) {
    $cmd .= "'$_' ";
}

system("$cmd"); # don't fail because utime fails on windows

open A, ">", "$basename,$date.msg" or die "failed to open $basename-$date.msg";

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
