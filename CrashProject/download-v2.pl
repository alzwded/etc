#!/usr/bin/perl

my $idx = 1;
my $continue = 0;

my $dir = "/media/win_d/Crash";

if(-f "left") {
        open A, "left";
        $idx = <A>;
        chomp $idx;
        $continue = <A>;
        chomp $continue;
        close A;
}

for(; $idx <= 98; ++$idx) {
        my $tno = $idx;
        $tno = "0" . $tno if $idx < 10;

        if($continue < 2) {
            my $cmd = "wget ";
            $cmd .= "-P $dir ";
            $cmd .= "--no-parent ";
            $cmd .= "-r ";
            $cmd .= "-c ";
            $cmd .= "ftp://ftp.worldofspectrum.org/pub/sinclair/magazines/Crash/Issue";
            $cmd .= $tno;
            $cmd .= "/";
            print "running '$cmd'\n";

            open B, ">", "left";
            print B "$idx\n1\n";
            close B;

            system($cmd);

            open B, ">", "left";
            print B "$idx\n2\n";
            close B;
        }

        if($continue < 3) {
            print "moving files . . . ";
            system("mv -f $dir/ftp.worldofspectrum.org/pub/sinclair/magazines/Crash/Issue$tno $dir");
            print "done\n";

            open B, ">", "left";
            print B "$idx\n3\n";
            close B;
        }

        # pack archive
        print "packing cbr . . . ";
        system("cd $dir/Issue$tno/ && rar a $dir/Issue$tno.cbr *");

        # done. Yupee!
        print "done with issue#$idx. Deleting it\n";

        system("rm -rf $dir/Issue$tno/");

        $continue = 0;
}
