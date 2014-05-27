#!/usr/bin/env perl -w

use HTTP::Tiny;

my ($file) = @ARGV;

open A, "<$file";

while(<A>) {
    chomp;
    my $title = $_;

    print "     downloading $title (hopefully)\n";

    next if -f "$title.epub";

    my $url = "http://manybooks.net/_scripts/send.php?tid=$title&book=1:epub:.epub:epub";

    while(1) {
        # post!
        my $url = "http://manybooks.net/_scripts/download-ebook.php";#?tid=$title&book=1:epub:.epub:epub";
        my %args = ( tid => $title, book => "1:epub:.epub:epub", );

        print "      posting to $url: $args{tid}&$args{book}\n";

        my $request = HTTP::Tiny->new();
        my $response = $request->post_form($url, \%args);
        if($response->{success}) {
            #print $response->{content};
            #die 'testing';
            last;
        } else {
            print "failed to download $title; retrying in 30s...\n";
            sleep 30;
            next;
        }
    }

    my $count = 0;
    while(1) {
        print "     downing $url\n";
        my $hr = system("curl -L \"$url\" > \"$title\".epub");
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
