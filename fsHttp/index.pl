#!/usr/bin/perl

use URI::Escape;

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
    if(!open FIN,"<$path") {
        print "HTTP/1.1 404 Not Found\n";
        print "Content-Type: text/plain\n\n";
        print "cannot access $path ".`groups`."\n";
        exit 0;
    }
    binmode(STDOUT);
    binmode(FIN);

    my $offset = 0;
    my $length = 0;
    undef $offset;
    undef $length;
    if(defined $args{offset}) {
        $offset = $args{offset};
    }

    # TODO read Range, send Accept-Range, send Content-Range: start-end/total, send Content-Length, limit output to 4M or something like that
    if(defined $ENV{HTTP_RANGE}) {
        my $s = $ENV{HTTP_RANGE};
        $s =~ m/(\d*)-(\d*)/;
        $offset = $1;
        $length = $2;
        # TODO normalize, validate
    }

    my $fileSize = `stat -c%s "$path"`;
    chomp $fileSize;
    if(defined $length && $length > $fileSize) {
        print "HTTP/1.1 400 Bad Request\n";
        print "Content-Type: text/plain\n\n";
        print "bad Range: request\n";
        exit 0; # TODO fail more elegantly
    } elsif(!defined $length) {
        if($offset + 4 * 1024 * 1024 < $fileSize) {
            $length = 4 * 1024 * 1024;
        } else {
            $length = $fileSize;
        }
    }

    if(defined $offset && $length > $offset + 4 * 1024 * 1024) {
        $length = $offset + 4 * 1024 * 1024;
    }

    if(defined $length && $length < $fileSize) {
        print "HTTP/1.1 206 Partial Content\n" if defined $length && $length < $fileSize;
        if(defined $offset) {
            if(defined $length) {
                print "Content-Range: $offset-$length/$fileSize\n";
            } else {
                print "Content-Range: $offset-$fileSize/$fileSize\n";
            }
        } else {
            print "Content-Range: 0-$length/$fileSize\n";
        }
    } elsif(!defined $offset && $length == $fileSize) {
        print "HTTP/1.1 200 OK\n";
        print "Content-Length: $fileSize\n";
    } else {
        print "HTTP/1.1 206 Partial Content\n";
        print "Content-Range: $offset-$fileSize/$fileSize\n" if defined $offset;
        print "Content-Range: 0-$fileSize/$fileSize\n" if !defined $offset;
    }
    print "Accept-Ranges: bytes\n";
    print "Content-Type: ".`file -L -b --mime-type "$path"`."\n";

    my $read = 0;
    my $br = 0;
    seek FIN, $offset, SEEK_SET if(defined $offset);
    $read += $offset if defined $offset;
    while ($br = read FIN, $buffer, 4096) {
        if(defined $length && $read > $length) {
            print substr $buffer, 0, $length - $offset;
            last;
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

    if(!opendir A, $path) {
        print "<p>Failed to open $path</p></body></html>";
        exit 0;
    }
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
