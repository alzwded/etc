#!/usr/bin/perl -w

print "<html><head><style>";
print 'p { margin-top: 2px; margin-bottom: 2px; }';
print "</style></head><body style='";
print 'font-family: "Consolas", "Lucida Console", monospace;';
print 'font-size: 12;';
print "'>\n";

my $alreadyPrinted = 0;
my $convertSpaces = 0;
my $currentLine = undef;
my @current = ();

my $state = 0;
<<EOT
used to convert *.notes file to html as per notes.vim syntax highlighting
    0 => default
    1 => :: ... ::
    2 => ^ ... :=
    3 => ! ... ! | ! ... .
    4 => % ... %
    5 => # ... \n
EOT
;

getnext();
&defaultp();

sub output {
    my ($c) = @_;
    if($c !~ m/[ \t\n]/) { $convertSpaces = 0; }
    if($c eq '<') { return "&lt;"; }
    if($c eq '>') { return "&gt;"; }
    if(!$convertSpaces) { return $c; }
    if($c eq ' ') { return "&nbsp;"; }
    if($c eq "\t") { return "&nbsp;" x 4; }
    return $c;
}

sub defaultp {
    while(1) {
        getnext();

        if($currentLine =~ m/^.*[:][=]/) {
            &def();
            $currentLine = join @current;
            next;
        } elsif(scalar(@current) >= 2 and $current[0] eq ':' and $current[1] eq ':') {
            &doublecolon();
            next;
        } elsif($current[0] eq '!') {
            &exclamation();
            next;
        } elsif($current[0] eq '%') {
            &percent();
            next;
        } elsif($current[0] eq '#') {
            &comment();
            next;
        }
        
        getnext();
        if(scalar(@current) >= 2 and $current[0] eq '<' and $current[1] eq '-') {
            print '<span style="color: darkgreen; font-weight: bold;">&#8592</span>';
            shift @current;
            shift @current;
            next;
        } elsif(scalar(@current) >= 2 and $current[0] eq '-' and $current[1] eq '-') {
            print '<span style="color: darkgreen; font-weight: bold;">&mdash;</span>';
            shift @current;
            shift @current;
            next;
        }
        my $c = $current[0];
        if($c =~ m|[.,;/*\-+]|) {
            print '<span style="font-weight: bold; color: red;">' . &output($c) . '</span>';
        } else {
            print &output($c);
        }

        shift @current;

    }
}

sub doublecolon {
    print '<span style="font-weight: bold; font-size: larger;">';
    shift @current;
    shift @current;
    while(1) {
        getnext();
        if(scalar(@current) > 2 and $current[0] eq ':' and $current[1] eq ':') {
            print("</span>");
            shift @current;
            shift @current;
            return;
        }
        print &output(shift @current);
    }
}

sub def {
    print '<span style="font-weight: bold; color: red;">';
    while(1) {
        getnext();
        if($current[0] eq ':' and $current[1] eq '=') {
            print(":=</span>");
            shift @current;
            shift @current;
            return;
        }
        print &output(shift @current);
    }
}

sub exclamation {
    print '<span style="font-weight: bold; color: navy;">';
    shift @current;
    while(1) {
        getnext();
        if($current[0] eq '!' or $current[0] eq '.') {
            print("</span>");
            shift @current;
            return;
        }
        print &output(shift @current);
    }
}

sub percent {
    print '<span style="font-weight: bold;"><i>';
    shift @current;
    while(1) {
        getnext();
        if($current[0] eq '%') {
            print("</i></span>");
            shift @current;
            return;
        }
        print &output(shift @current);
    }
}

sub comment {
    print '<span style="color: #404040;"><i><sub>';
    shift @current;
    while(1) {
        getnext();
        if($current[0] eq "\n") {
            print("</sub></i></span>");
            shift @current;
            return;
        }
        print &output(shift @current);
    }
}

sub getnext {
    if(scalar(@current) == 0) {
        if($alreadyPrinted) { print("</p>\n"); }
        else { $alreadyPrinted = 1; }
        $currentLine = <STDIN>;
        if(!$currentLine) { &doexit(); }
        @current = split //, $currentLine;
        print('<p>');
        $convertSpaces = 1;
    } else {
        #print defined(@current);
        #print "\n" . scalar(@current) . "\n";
    }
}

sub doexit {
    print '</body></html>'."\n";
    exit 0;
}
