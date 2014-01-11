#!/usr/bin/perl

my $idx = 1;
my $continue = 0;

my $dir = "/media/win_d/SinclairUser";
my $remoteDir = "ftp.worldofspectrum.org/pub/sinclair/magazines/SinclairUser/Issue";
my $startIndex = 1;
my $noIssues = 134;
my $numWidth = 3;
$idx = $startIndex;

if(-f "left") {
        open A, "left";
        $idx = <A>;
        chomp $idx;
        $continue = <A>;
        chomp $continue;
        close A;
}

for(; $idx <= ($startIndex + $noIssues); ++$idx) {
        my $tno = pad($idx);
        
        my $continueQuestionMark = system("stat $dir/Issue$tno.cbr > /dev/null");
        if($continueQuestionMark == 0) {
                next;
        }

        if($continue < 2) {
            my $cmd = "wget ";
            $cmd .= "-P $dir ";
            $cmd .= "--no-parent ";
            $cmd .= "-r ";
            $cmd .= "-c ";
            $cmd .= "ftp://$remoteDir";
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
            system("mv -f $dir/$remoteDir"."$tno $dir");
            print "done\n";

            open B, ">", "left";
            print B "$idx\n3\n";
            close B;
        }

        # pack archive
        print "packing cbr . . . ";
        system("cd $dir/Issue$tno/ && \\nice -n 10 rar a $dir/Issue$tno.cbr *");

        # done. Yupee!
        print "done with issue#$idx. Deleting it\n";

        system("rm -rf $dir/Issue$tno/");

        $continue = 0;
}

sub pad {
        my ($idx) = @_;
        my $tno = "";
        my $num = int(log($idx)/log(10)) + 1;
        if($num < $numWidth) {
               $tno = ("0" x ($numWidth - $num)) . $idx;
        }
        return $tno;
}
