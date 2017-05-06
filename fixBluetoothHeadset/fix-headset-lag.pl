#!/usr/bin/perl

=pod
Yet another script that fixes an annoying problem on linux...

Have you ever had a bluetooth headset that is lagging 0.5-1s? It's probable
that you need to manually reduce said lag to something more reasonable.
=cut


my $s = `pactl list cards short | grep bluez`;

my @lines = grep { $_ !~ /^$/ } split /$/, $s;

die 'too many cards'."\n" if(0 + @lines > 1);
die 'headset not connected?' if(0 + @lines == 0);

my $line = $lines[0];
print "$line\n";
$line =~ m/^\d+\s+([^ \t\n]+).*$/g or die "can't read card";
my $card = $1;
my $sink = $card;
my $source = $card;
$sink =~ s/_card/_sink/;
$source =~ s/_card/_source/;

print "$card\n";


#echo profile1 && pactl set-card-profile $card a2dp_sink && \
#echo profile2 && pactl set-card-profile $card headset_head_unit && \
#echo profile3 && pactl set-card-profile $card a2dp_sink && \

(system(<<EOT 
echo profile1 && pactl set-card-profile $card a2dp_sink && \
echo set sink && pactl set-default-sink $sink.a2dp_sink && \
echo set latency && pactl set-port-latency-offset $card headset-output 500000
EOT
) == 0) or die 'pactl failed';
