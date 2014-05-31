package second;

sub init {
}

sub exec {
    my ($param) = @_;

    print "second running with $param\n";
}

sub enum {
    return ("asd", "qwe");
}

print "second init'd\n";
1;
