#!/usr/bin/perl -w

open my $fh, "LINKS" or die 'failed to open LINKS file';

$SIG{'INT'} = "interrupt";
$SIG{'QUIT'} = "interrupt";

my @children = ();

print "spawning tasks\n";
while(<$fh>) {
    chomp;
    #system("streamripper '$_' -M 500 &");
    my $pid = fork();
    if($pid < 0) {
        killChildren();
        die 'failed to fork';
    } elsif($pid == 0) {
        exec "streamripper '$_' -M 500" or die 'failed to exec';
    } else {
        print "    spawned process $pid\n";
        push @children, $pid;
    }
}

close $fh;

while(1) {}

exit 0;

sub killChildren {
    print "killing all streamrippers\n"
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
