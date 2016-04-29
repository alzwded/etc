#!/usr/bin/perl -w
use List::Util qw/first/;
my @regs = (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

@regs = map { $_ + 31 } @regs;
@regs[0, 3, 5, 9] = map { $_ + 76 } @regs[0, 3, 5, 9];
my $f = sub { return ${$_[0]}+1 };
foreach(@regs) { my $wha = $_; $regs[first { $regs[$_] == $wha } 0..$#regs] = &$f(\$wha) }
@regs[1, 8] = (${\@regs}[1] + ord("a") - ord(" "), ${\@regs}[1] + ord("a") - ord(" "));
foreach((3, 9, 9, 10)) { $regs[$_ + 0] = &$f(\($regs[$_] + 0)) }
&{sub { ${$_[0]}++; ++${$_[1]} }}(\$regs[4], \$regs[6]) foreach (-47)..(36);
$regs[4] += $regs[4] =~ /\d/;

print join('', map { chr $_ } @regs). "\n";
