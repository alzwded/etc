#!/usr/bin/perl
# Copyright Vlad Meșco 2022
# 
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Jak's fake grep
# -- for when the 'grep' or 'egrep' or 'fgrep' that's in path is so old
#    that '^.\{1,20\}$' nor "^.{1,20}$" do what they should.

use strict;

use Getopt::Long;
use File::Spec;
my $expression = undef;
my $recursive = undef;
my $path = undef;
my $ignorecase = undef;
my $Pathsep = undef;
my $nativePathsep = undef;
my $Fullpath = undef;
my $reverse = undef;
my $showHelp = undef;
my $exitcode = 1;

#print STDERR "Jak's fake grep\n"; # proof/confirmation we have a modern re engine # too annoying to have it on

# this nicely matches -e, -r and -i from real grep
my %options = (
           "expression=s" => \$expression,
           "recursive" => \$recursive,
           "ignorecase" => \$ignorecase,
           "Pathsep=s" => \$Pathsep,
           "Fullpath" => \$Fullpath,
           "v" => \$reverse,
           "help" => \$showHelp);
sub showHelpHelper {
    my ($lambda) = @_;
    use Data::Dumper;
    print Data::Dumper->Dump([[sort keys %options]], [qw/options/]);
    &$lambda() if $lambda;
}
GetOptions(%options) or showHelpHelper(sub { die("Error in command line arguments" ) });
showHelpHelper(sub { exit 1 }) if defined $showHelp;

#use Data::Dumper;
#print Dumper(\@ARGV);

# sometimes on Cygwin or WSL I want to force Windows path separators to 
# cut-paste paths into some windows app
if(defined $Pathsep) {
    die "Pathsep must be / or \\ or undefined; you provided `$Pathsep'" if($Pathsep !~ m/^(\\|\/)$/s);
    $nativePathsep = File::Spec->catfile('','');
    $Pathsep = $Pathsep;
    if($nativePathsep eq $Pathsep) {
        # no point in transforming if they're the same
        $nativePathsep = undef;
        $Pathsep = undef;
    }
}

# get expression from "rest" if not already specified, like real grep
if(! defined($expression)) {
    $expression = shift;
}

if(! defined($expression) || length($expression) == 0) {
    die('missing expression');
}

# get path from "rest", like real grep
if(! defined($path)) {
    my $s = shift;
    if(defined($s) && length($s) > 0)
    {
        $path = $s unless ($recursive && $s eq '.');
    }
}
$path = undef if length $path == 0;
$path = '.' if (!defined($path) && $recursive);

if(!$recursive) {
    do_file($path);
} else {
    do_dir($path);
}

exit $exitcode;

# if we came from a file, print path, line number and matched line
sub pprint {
    my ($file, $line, $s) = @_;
    if(defined $file) {
        my $pfile = File::Spec->canonpath($file);
        $pfile = File::Spec->rel2abs($pfile) if defined $Fullpath;
        $pfile =~ s#\Q$nativePathsep\E#$Pathsep#g if defined $Pathsep;
        print( "$pfile($line): ");
    }
    print $s."\n";
}

sub do_file {
    my ($path) = @_;

    my $fh;
    if(defined($path)) {
        my $hr = open $fh, "<", $path;
        if(!$hr) {
            if(-e $path) {
                print STDERR "grep: $path: Permission denied\n";
            } else {
                print STDERR "grep: $path: No such file\n";
            }
            return;
        }
    } else {
        $fh = \*STDIN;
    }

    my $line = 1;
    while(<$fh>) {
        chomp;
        my $s = $_;
        my $print = 0;
        if(!defined $reverse) {
            if($ignorecase) {
                $print = 1 if $s =~ /$expression/i;
            } else {
                $print = 1 if $s =~ /$expression/;
            }
        } else {
            if($ignorecase) {
                $print = 1 unless $s =~ /$expression/i;
            } else {
                $print = 1 unless $s =~ /$expression/;
            }
        }
        pprint($path, $line, $s) if $print;
        $exitcode = 0 if $print;
        ++$line;
    }

    if(defined($path)) {
        close $fh;
    }
}

sub do_dir {
    my ($path) = @_;

    my $hr = opendir(my $dh, $path);
    if(!$hr) {
        if(-e $path) {
            print STDERR "grep: $path: Permission denied\n";
        } else {
            print STDERR "grep: $path: No such directory\n";
        }
        return;
    }

    while(readdir $dh) {
        next if($_ eq '.' || $_ eq '..');
        my $p = "$path/$_";
        if(-d $p) {
            do_dir($p);
        } else {
            do_file($p);
        }
    }

    closedir($dh);
}
