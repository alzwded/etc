package hello;

sub init {
}

sub exec {
    my ($param) = @_;

    print "hello running with $param\n";
}

sub enum {
    return ("lol", "kik");
}

print "hello init'd\n";
1;
