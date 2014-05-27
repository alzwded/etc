#!/usr/bin/env perl -w

use HTTP::Tiny;

my ($file) = @ARGV;

open A, "<$file";

while(<A>) {
    chomp;
    my $title = $_;

    next if -f "$title.epub";

    while(1) {
        # post!
        my $url = "http://manybooks.net/_scripts/download-ebook.php?tid=$title&book=1:epub:.epub:epub";
        #my %args = ( tid => $title, book => "1:epub:.epub:epub" );

        my $request = HTTP::Tiny->new();
        my $response = $request->request("POST", $url);
        if($response->success) {
            last;
        } else {
            print "failed to download $title; retrying in 30s...\n";
            sleep 30;
            next;
        }
    }

    my $url = "http://manybooks.net/_scripts/spend.php?tid=$title&book=1:epub:.epub:epub";

    my $count = 0;
    while(1) {
        my $hr = system("curl \"$url\" > \"$title\".epub");
        if($hr == 0) { last; }
        if($count < 30) {
            $count++;
            print "something went wrong; retrying in 30s...\n";
            sleep 30;
            next;
        } else {
            die 'something went horribly wrong!';
        }
    }
}
