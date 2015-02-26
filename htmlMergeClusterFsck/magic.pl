#!/usr/bin/perl -w

use strict;
use feature "switch";

my @frames = ();

open A, "index.html";
open B, ">new_index.html";

my $state = 0;

asd: while(<A>) {
    chomp;
    my $line = $_;


    for($state) {
    when(0) {
        if($line =~ /<\s*a\s*href="([^]]*)"\s*>/) {
            my $lnk = $1;

            if(-f $lnk) {
                push @frames, $lnk;
            }
        }
        if($line =~ m/<!-- IFRAMESHERE -->/) {
            my $numFrames = scalar(@frames);
            print B <<EOT ;
<script type="text/javascript">
counter = $numFrames
</script>
EOT

            foreach my $lnk (@frames) {
                my $fileContents;
                open my $fh, "$lnk";
                read $fh, $fileContents, 4096*1024*1024;
                close $fh;
#                print B <<EOT ;
#<!-- <iframe src="$lnk" onload="increment()"></iframe> -->
#<object data="$lnk" onload="increment()"></object>
#EOT
                print B <<EOT ;
<div class="ffs_t" style="visibility:hidden;display:none" onload="increment()">$fileContents</div>
EOT
            }

            print B '<span id="bookContent" />';
            print B <<EOT ;
<script type="text/javascript" src="extract.js" defer></script>
EOT
            $state = 1;
            next asd;
        }
        print B "$line\n";
    } # when(0)
    when(1) {
        print B "$line\n";
    } # when(1)
    } # for($state)
}

close A;
close B;
