#!/usr/bin/perl -w

use strict;
use Getopt::Long;
#use IPC::Open2; # BROKEN
#use IO::Select; # Only works on sockets, BROKEN
#use POSIX ":sys_wait_h";
#use POSIX qw(:errno_h);
use File::Temp qw/tempfile/;

my @titles = ();
my ($index, $bz2);
my @queries = ();
my $verbose = 1;

GetOptions( "title=s@" => \@titles,
            "query=s@" => \@queries,
            "verbose" => \$verbose,
            "index=s" => \$index,
            "bz2=s" => \$bz2 ) or die('error parsing arguments');

@titles = split(/,/, join(',', @titles));

die('you need to specify titles') unless @titles || @queries;
die('you need to specify the path to the index file') unless -f $index;

my @indexes = ();
open(INDEX, $index) or die("can't open $index");
while(<INDEX>) {
    chomp;
    if($_ =~ m/(\d+):(\d+):(.*)/) {
    #&& grep { $_ eq $3 } @titles) {
        push @indexes, {
            title => $3,
            offset => $1,
            pageid => $2
        };
        if(@queries) {
            my @res = grep { my $title = $_->{title}; grep { $title =~ m/$_/ } @queries } ($indexes[@indexes - 1]);
            if(@res) { print "$res[0]->{title}\n"; }
        }
    }
}
close(INDEX);

my %toProcess = ();
for(my $i = 0; $i < @indexes; ++$i) {
    if( grep { $_ eq $indexes[$i]{title} } @titles) {
        my $thing = $indexes[$i];
        for(my $j = $i + 1; $j < @indexes; ++$j) {
            if($indexes[$j]->{offset} != $indexes[$i]->{offset}) {
                $thing->{end} = $indexes[$j]->{offset};
                last;
            }
        }
        $toProcess{$thing->{title}} = $thing;
    }
}


use Data::Dumper;
print Dumper \%toProcess;

if(%toProcess == 0) { exit 0; }
die('you need to specify the path to the multistream bz2 file') unless -f $bz2;

open(BZ2, "<:raw", $bz2) or die("can't open $bz2");

my @output = ();
foreach my $title (@titles) {
    my $h = $toProcess{$title};
    # extract stream
    if(!defined($h->{end})) {
        seek BZ2, 0, 2;
        $h->{end} = tell BZ2;
    }
    seek BZ2, $h->{offset}, 0;
    my $bytes; # c.f. my @bytesArray = unpack $bytes;
    my $bytesRead = sysread BZ2, $bytes, $h->{end} - $h->{offset};
        
    die("don't know how to handle reading less than the expected number of bytes") if($bytesRead < $h->{end} - $h->{offset});

    my ($fh, $filename) = tempfile(UNLINK => 1);
    binmode $fh;
    print $fh $bytes;

    print "Launching bzip2 for $h->{title}\n" if $verbose;
    my $bzipoutput = `bzip2.exe -d<$filename`;
    close($fh);

    # find my page
    my @hacked = split /<title>/, $bzipoutput;
    my $tit = quotemeta $h->{title};
    my @found = grep { $_ =~ m/^$tit<\/title/ } @hacked;
    if(@found) {
        my $thing = shift @found;
        $thing =~ m/<text[^>]*>(.*?)<\/text/s;
        push @output, split(/\n/, $1);

    }

    print "Done with $h->{title}\n\n";
}
        print "7 sample lines\n";
        foreach my $i (0..9) {
            print "$output[$i]\n";
        }

print "This is the end\n";
close BZ2;

my ($fh, $filename) = tempfile(UNLINK => 0);
print $fh join("\n", @output);
print $filename;
my $titles = join(',', @titles);
#my $html = `c:\\php7\\php.exe mediawiki-1.32.0\\maintenance\\parse.php --title="$titles" --conf=..\\..\\LocalSettings.php<$filename>output.html`;

if(not -f "parse.bat") {
    open BAT, ">parse.bat";
    print BAT <<EOF ;
\@echo off
SET FILE=%~f1
SETLOCAL
pushd mediawiki-1.32.0
c:\\php7\\php.exe maintenance\\parse.php --title="$titles"<"%FILE%">..\\output.html
ENDLOCAL
EOF
    close BAT;
}
print `cmd.exe /c parse.bat "$filename"`;
close($fh);
