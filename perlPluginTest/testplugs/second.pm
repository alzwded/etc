package second;

sub init {
    print "second init'd\n";
}

sub exec {
    my ($param) = @_;

    print "second running with $param\n";
}

sub enum {
    return ("asd", "qwe");
}

1
