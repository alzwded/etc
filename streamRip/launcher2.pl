#!/usr/bin/perl -w

use POSIX ();

open my $fh, "LINKS" or die 'failed to open LINKS file';

$SIG{'INT'} = "interrupt";
$SIG{'QUIT'} = "interrupt";
$SIG{'CHLD'} = "reap";

my @children = ();

print "spawning tasks\n";
while(<$fh>) {
    chomp;
    my $link = $_;
    print "    forking for $link";
    my $pid = fork();
    if($pid < 0) {
        killChildren();
        die 'failed to fork';
    } elsif($pid == 0) {
        exec "streamripper '$link' -M 500" or die 'failed to exec';
    } else {
        print "    spawned process $pid for $link\n";
        push @children, $pid;
    }
}

close $fh;

while(1) {
    exit 255 if scalar(@children) == 0;
    sleep 5;
}

exit 0;

sub killChildren {
    print "killing all streamrippers\n";
    #system("killall -INT streamripper");
    foreach my $i (@children) {
        print "    interrupting $i\n";
        kill 2, $i;
    }
}

sub interrupt {
    killChildren();
    print "exiting\n";
    exit 0;
}

sub reap {
    while() {
        my $pid = waitpid -1, POSIX::WNOHANG;
        last if $pid <= 0;
        for(my $i = 0; $i < scalar(@children); ++$i) {
            if($pid == $children[$i]) {
                splice @children, $i, 1;
                last;
            }
        }
        print "process $pid finished\n";
    }

    print "no more children...\n" if scalar(@children) == 0;
}
