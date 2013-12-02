#!/usr/bin/perl

use URI::Escape;
use CGI;

payload();

sub payload {
    our %args = ();
    foreach my $a ( split /&/, $ENV{QUERY_STRING}) {
        $a =~ /^([^=]*)=(.*)$/;
        $args{$1} = $2;
    }

    my $path = "./";
    if(defined $args{p}) { $path = uri_unescape($args{p}); }

    if(-d $path) {
        index_page($path);
    } else {
        binary_page($path);
    }

}

sub binary_page {
    my ($path) = @_;
    while(-l $path) {
        $path = readlink($path);
    }
    my $buffer;
    print "Content-Type: ".`file -L -b --mime-type "$path"`."\n";

    open FIN,"<$path" or print "cannot access $path ".`groups`."\n";
    binmode(STDOUT);
    binmode(FIN);

    my $offset = 0;
    undef $offset;
    if(defined $args{offset}) {
        $offset = $args{offset};
    }

    # TODO read Accept-Range, send Content-Range: start-end/total, send Content-Length, limit output to 4M or something like that
    #my $q = CGI->new();
    #if(defined $q->http(

    my $read = 0;
    my $br = 0;
    while ($br = read FIN, $buffer, 4096) {
        if(defined $offset) {
            $read += $br;
            if($read <= $offset) {
                next;
            } else {
                $offset -= $read;
                #$buffer = splice $buffer, $offset;
                $buffer = substr $buffer, $offset;
                undef $offset;
            }
        }

        print $buffer;
    }
    close FIN;
}

sub index_page {
    my ($path) = @_;
    print <<EOF
<html><head><title>Jak's stuff</title></head>
<body>
EOF
;

    opendir A, $path or die 'sorry, can\'t do that';
    my @files = readdir A;
    closedir A;

    foreach (@files) {
        next if $_ =~ m/^\.$/;
        my $uifile = $_;
        my $file = $_;
        if($file =~ m/^\.\.$/) {
            $file = $path;
            $file =~ s|/+|/|g;
            $file =~ s|(.*/)(.+)|$1|;
        } else {
            $file = $path.'/'.$_;
            $file =~ s|/+|/|g;
        }
        
        my $link = uri_escape($file);
        print "<p>";
        print '[' if (-d $file);

        print "<a href=\"index.pl?p=$link\">";

        print $uifile;
            
        print "</a>";
        print ']' if (-d $file);
        print "</p>";
    }

    print "</body></html>";
}
