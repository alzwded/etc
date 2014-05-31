#!/usr/bin/perl -w

#my %plugins = ();
my @plugins = ();
my %mappings = ();

&load;

foreach (@plugins) {
    my $plug = $_;
    print "works with: ";
    foreach ($plug->enum()) {
        my $a = $_;
        print $a." ";
        %mappings = (%mappings, $a => $plug);
    }
    print "\n";
    # $plug->exec();
}

while(1) {
    my $line = readline;
    last if(!defined($line) or $line =~ m/^$/);

    chomp $line;

    if(defined $mappings{$line}) {
        $mappings{$line}->exec();
    } else {
        print "no plugin that can handle '$line' loaded\n";
    }
}

sub load {
    my @plugs = ();
    opendir A, "testplugs";
    my @plugs = map { "./testplugs/".$_ ; } grep { $_ =~ m/^.*\.pm$/ ; } readdir A;
    closedir A;

    foreach (@plugs) {
        my $a = $_;
        eval {
            require $a;
            $a =~ s#^\./testplugs/(.*)\.pm$#$1#;
            $a->import();
        };
        if ($@) {
            print "can't load $a\n";
            next;
        }
        #push %plugins, (("" + $a) => $a);
        push @plugins, $a;
    }
}
