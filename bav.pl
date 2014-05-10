#!/usr/bin/perl -w

use Cwd;

my @files = @ARGV;

die "usage: bav.pl files" unless scalar @files > 0;

if($files[0] eq '-l' && scalar(@files) == 2) {
    my @messages = sort { $b cmp $a } split /\n/, `ls -1 '$files[1],'[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]'.msg'`;

    foreach (@messages) {
        next if $_ eq '';
        my $file = $_;
        $file =~ m/^$files[1],(.*)\.msg$/;
        my $date = $1;
        $date =~ /([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})/;
        my $nicedate = "$1/$2/$3 $4:$5:$6";
        print "$nicedate:\n";
        catfile($file);
        print(("-" x 72)."\n");

    }

    exit 0;
} elsif($files[0] eq '-l' && scalar(@files) == 1) {
    my @messages = sort { $b cmp $a } split /\n/, `ls -1 *','[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]'.msg'`;

    foreach (@messages) {
        next if $_ eq '';
        my $file = $_;
        $file =~ m/^(.*),(.*)\.msg$/;
        my $date = $2;
        my $basename = $1;
        $date =~ /([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})/;
        my $nicedate = "$1/$2/$3 $4:$5:$6";
        print "$nicedate: $basename\n";
        catfile($file);
        print(("-" x 72)."\n");

    }

    exit 0;
}

my $basename = cwd();
if(scalar @files == 1) {
    $basename = $files[0];
}

$basename =~ s#^.*/([^/]*)\.?.*$#$1#;

my $date = getdate();
my $cmd = getarchcommand($basename, $date);
foreach (@files) {
    die if !-e $_;
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

sub getdate {
    my $date = `date +%Y%m%d%H%M%S`;
    chomp $date;
    return $date;
}

sub getarchcommand {
    my ($basename, $date) = @_;
    my $cmd = "tar cjf '$basename,$date.tbz' ";
    return $cmd;
}

sub catfile {
    my ($file) = @_;
    system "cat '$file'";
}
