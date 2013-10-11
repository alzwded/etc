#!/usr/bin/perl -w

use Term::ReadKey;

our $V = 1;

opendir A, "." or die 'cannot read current directory';
my @files = readdir A;

my $nbBuckets = int(scalar(@files) / 30);
$nbBuckets = 1 if $nbBuckets < 1;

print "buckets: $nbBuckets\n" if defined($V);

my @buckets = ();
my @bucketNumbers = ();
my @filled = ();
my @filledNumbers = ();

for(my $i = 0; $i < $nbBuckets; ++$i) {
    my @a = ();
    print  "pushing bucket $i\n" if defined($V);
    push @buckets, \@a;
    push @bucketNumbers, $i;
}

foreach my $file (@files) {
    next if $file =~ m/^\.$/ || $file =~ m/^\..*$/;
    
    my $n = int(rand($nbBuckets));

    print "placing $file in bucket ".$bucketNumbers[$n]."\n" if defined($V);

    push @{$buckets[$n]}, $file;

    if($nbBuckets > 1 && scalar(@{$buckets[$n]}) > 30) {
        $nbBuckets--;
        push @filled, $buckets[$n];
        push @filledNumbers, $bucketNumbers[$n];
        print "removing bucket ". $bucketNumbers[$n] ." from bucket pool\n" if defined($V);
        splice @buckets, $n, 1;
        splice @bucketNumbers, $n, 1;
    }
}

print "joining filled buckets with bucket pool\n" if defined $V;
push @buckets, @filled;
push @bucketNumbers, @filledNumbers;

my $idx = 0;
foreach my $bucket (@buckets) {
    print "bucket ".$bucketNumbers[$idx++].":\n";

    my $idx = 1;
    foreach my $file (@{ $bucket }) {
        printf "%s%2d. %s\n", " " x 4, $idx++, $file;
    }

    print "\n";
}

ReadMode 4;
END { ReadMode 0; }
while() {
    print "continue? [Yn]\n";
    my $y;
    while(!defined ($y = ReadKey(-1))) {};
    die 'eof, not cool...' if !defined($y);
    exit 0 if $y eq 'n' || $y eq 'N';
    last if $y eq 'y' || $y eq 'Y';
}
ReadMode 0;

my $idx = 0;
foreach my $bucket (@buckets) {
    system("mkdir -p \"bucket-" . $bucketNumbers[$idx] . "\"") == 0 or die 'cannot create directory';

    foreach my $file (@{ $bucket }) {
        system("mv \"$file\" \"bucket-". $bucketNumbers[$idx] ."\"") == 0 or die "cannot move file $file";
    }

    ++$idx;
}

exit 0;
