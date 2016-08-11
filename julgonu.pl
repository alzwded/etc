#!/usr/bin/perl -w
#Copyright (c) 2016, Vlad Mesco
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
#1. Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#2. Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
#ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

use strict;
use feature "switch";
no warnings qw/experimental::smartmatch/;

my @states = ();
my %syms = (
    0 => '.',
    1 => '1',
    2 => '2',
    3 => '3',
    4 => '4',
    13 => 'z',
    14 => 'x',
    15 => 'c',
    16 => 'v'
);
my @board = ();
my $trn = 0;

init();

printBoard();
while(<STDIN>)
{
    chomp;
    interpret($_);
    printBoard();
}

sub zeroOrOne {
    my ($idx) = @_;
    $syms{$idx} || $syms{0}
}

sub init {
    @board = map { zeroOrOne($_) } (1..16);
    $trn = 0;
    @states = ();
    #pushState();
}

sub printBoard {
    print "\n";
    $trn %= 2;
    for ($trn) {
        when (0) { print " zxcv's turn\n" }
        when (1) { print "1234's turn\n" }
    }
    print ("/".("-" x 12)."\\\n");
    for(my $i = 0; $i < 4; ++$i) {
        print "|";
        for(my $j = 0; $j < 4; ++$j) {
            print " $board[$i * 4 + $j] ";
        }
        print "|\n";
    }
    print ("\\".("-" x 12)."/\n> ");
}

sub interpret {
    my ($cmd) = @_;

    for ($cmd) {
        when (/^\s*\?\s*$/) { help() }
        when (/^\s*help\s*$/) { help() }
        when (/^\s*q\s*$/) { exit 0 }
        when (/^\s*r\s*$/) { init() }
        when (/^\s*(([u]\s*)+)$/) { popState($1) }
        when (/^\s*([zxcv])\s*([hjkl]+)\s*$/) {
            if($trn % 2 == 0) {
                move($1, $2);
            } else {
                print "wrong turn\n";
            }
        }
        when (/^\s*([1234])\s*([hjkl]+)\s*$/) {
            if($trn % 2 == 1) {
                move($1, $2);
            } else {
                print "wrong turn\n";
            }
        }
        default { print "Bad command or file name\n" }
    }
}

sub move {
    my ($what, $where) = @_;
    for(my $i = 0; $i < 16; ++$i) {
        if($board[$i] eq $what) {
            my $cnt = length $where;
            for($where) {
                when (/h+/) {
                    my $j = $i-$cnt;
                    if($i % 4 <= $cnt - 1
                    or 0 != scalar grep {$_ ne $syms{0}} @board[$j..$i-1]
                    ) {
                        print "cannot go there\n";
                        return;
                    }
                    pushState();
                    $board[$i] = $syms{0};
                    $board[$j] = $what;
                    checkCapture($j);
                    return
                }
                when (/j+/) {
                    my $j = $i + 4*$cnt;
                    if($j >= 16
                    or 0 != scalar grep {$_ ne $syms{0}} @board[map { $i + $_*4 } 1..$cnt]
                    ) {
                        print "cannot go there\n";
                        return;
                    }
                    pushState();
                    $board[$i] = $syms{0};
                    $board[$j] = $what;
                    checkCapture($j);
                    return
                }
                when (/k+/) {
                    my $j = $i - 4*$cnt;
                    if($j < 0
                    or 0 != scalar grep {$_ ne $syms{0}} @board[map { $i - $_*4 } 1..$cnt]
                    ) {
                        print "cannot go there\n";
                        return;
                    }
                    pushState();
                    $board[$i] = $syms{0};
                    $board[$j] = $what;
                    checkCapture($j);
                    return
                }
                when (/l+/) {
                    my $j = $i+$cnt;
                    if(($i % 4) + $cnt >= 4
                    or 0 != scalar grep {$_ ne $syms{0}} @board[$i+1..$j]
                    ) {
                        print "cannot go there\n";
                        return;
                    }
                    pushState();
                    $board[$i] = $syms{0};
                    $board[$j] = $what;
                    checkCapture($j);
                    return
                }
            }
        }
    }
    print "piece not found\n";
}

sub checkCapture {
    my ($i) = @_;
    #printf "%d %d %d\n", $i, $i - ($i%4), ($i - ($i%4) + 4);
    foreach (([$i-1, $i-2, sub { $_[0] < $i-($i%4)}],
              [$i-4, $i-8, sub { $_[0] < 0 }],
              [$i+1, $i+2, sub { $_[0] >= ($i-($i%4)+4) }],
              [$i+4, $i+8, sub { $_[0] >= 16 }])
    ) {
        my ($j, $k, $lim) = @{$_};
        #use Data::Dumper;
        #print Dumper ($j, $k, $lim);
        #print Dumper (&$lim($j));
        if(&$lim($j) || &$lim($k)) { next }
        #print "not skipped\n";
        if($board[$i] =~ m/[zxcv]/
        && $board[$j] =~ m/[1234]/
        && $board[$k] =~ m/[zxcv]/
        or $board[$i] =~ m/[1234]/
        && $board[$j] =~ m/[zxcv]/
        && $board[$k] =~ m/[1234]/)
        {
            #print "removed\n";
            $board[$j] = $syms{0};
            next;
        }
        #print "no catch\n";
    }
    foreach (([$i-1, $i-2, $i-3,  sub { $_[0] < $i-($i%4)}],
              [$i-4, $i-8, $i-12, sub { $_[0] < 0 }],
              [$i+1, $i+2, $i+3,  sub { $_[0] > ($i-($i%4)+4) }],
              [$i+4, $i+8, $i+12, sub { $_[0] >= 16 }])
    ) {
        my ($j, $k, $l, $lim) = @{$_};
        if(&$lim($j) || &$lim($k) || &$lim($l)) { next }
        if($board[$i] =~ m/[zxcv]/
        && $board[$j] =~ m/[1234]/
        && $board[$k] =~ m/[1234]/
        && $board[$l] =~ m/[zxcv]/
        or $board[$i] =~ m/[1234]/
        && $board[$j] =~ m/[zxcv]/
        && $board[$k] =~ m/[zxcv]/
        && $board[$l] =~ m/[1234]/)
        {
            $board[$j] = $board[$k] = $syms{0};
            next;
        }
    }

    if((scalar grep { $_ =~ m/[zxcv]/ } @board) <= 1) {
        print("1234 wins\n");
    }
    if((scalar grep { $_ =~ m/[1234]/ } @board) <= 1) {
        print("zxcv wins\n");
    }
}

sub pushState {
    push @states, { trn => $trn, board => [map { $_ } @board] };
    ++$trn;
}

sub popState {
    my ($a) = @_;
    $a =~ s/\s//g;
    my $cnt = length $a;
    foreach (1..$cnt) {
        return if 0 == @states;
        my $last = pop @states;
        $trn = $last->{trn};
        @board = @{$last->{board}};
    }
}

sub help {
    print <<EOT;
julgonu.pl Copyright (c) 2016, Vlad Mesco

?                       this message
r                       restart game
u                       undo to last state
q                       exit
<who> <motion>          move piece, where
    who ::= [zxcv]      player one
          | [1234]      player two
    motion ::= direction +
    direction ::= h     left
                | j     down
                | k     up
                | l     right
EOT
}
