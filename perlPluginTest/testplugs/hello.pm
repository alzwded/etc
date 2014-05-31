package hello;

sub init {
    print "hello init'd\n";
}

sub exec {
    my ($param) = @_;

    print "hello running with $param\n";
}

sub enum {
    return ("lol", "kik");
}

1
