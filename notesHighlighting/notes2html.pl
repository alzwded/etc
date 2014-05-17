#!/usr/bin/perl -w

print <<EOT
<!DOCTYPE html>
<html><head><title>NOTES</title><meta charset="utf-8"><style>
p.notes {
    margin-top: 2px; margin-bottom: 2px;
    font-family: "Consolas", "Lucida Console", monospace;
    font-size: 12;
}

p.notes .redchar {
    font-weight: bold;
    color: red;
}

p.notes .greenchar {
    font-weight: bold;
    color: green;
}

p.notes .def {
    font-weight: bold;
    color: red;
}

p.notes .exclamation {
    font-weight: bold;
    color: navy;
}

p.notes .doublecolon {
    font-weight: bold;
    font-size: larger;
}

p.notes .percent {
    font-weight: bold;
    font-style: oblique;
}

p.notes .comment {
    font-style: italic;
    color: #404040;
    position: relative;
    top: +0.2em;
    font-size: 80%;
}

</style></head><body>
EOT
;


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
    if($c eq '`') { return "&nbsp;"; }
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
            $currentLine = join '', @current;
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
            print '<span class="greenchar">&#8592;</span>';
            shift @current;
            shift @current;
            next;
        } elsif(scalar(@current) >= 2 and $current[0] eq '-' and $current[1] eq '-') {
            print '<span class="greenchar">&mdash;</span>';
            shift @current;
            shift @current;
            next;
        } elsif(scalar(@current) >= 2 and (join '', @current[0..1]) =~ m|[-=][>]|) {
            print '<span class="greenchar">'.output(shift @current).output(shift @current).'</span>';
            next;
        }
        my $c = $current[0];
        shift @current;
        if($c =~ m|[.,;/*\-+]|) {
            print '<span class="redchar">' . &output($c) . '</span>';
        } elsif($c =~ m#[[\](){}|&^@]#) {
            print '<span class="greenchar">' . &output($c) . '</span>';
        } else {
            print &output($c);
        }
    }
}

sub doublecolon {
    print '<span class="doublecolon">';
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
    print '<span class="def">';
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
    print '<span class="exclamation">';
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
    print '<span class="percent">';
    shift @current;
    while(1) {
        getnext();
        if($current[0] eq '%') {
            print("</span>");
            shift @current;
            return;
        }
        print &output(shift @current);
    }
}

sub comment {
    print '<span class="comment">';
    shift @current;
    while(1) {
        getnext();
        if($current[0] eq "\n") {
            print("</span>");
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
        print('<p class="notes">');
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
