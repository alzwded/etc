#!/usr/bin/perl -w

open A, ">manyBoxes";

my $size = 0.1;
my $L = $ARGV[0] || 20;

for(my $i = 0; $i < $L; ++$i) {
    for(my $j = 0; $j < $L; ++$j) {
        for(my $k = 0; $k < $L; ++$k) {
            printBox($i, $j, $k);
        }
    }
}

close A;

sub printBox {
    my ($i, $j, $k) = @_;
    printBase($i, $j, $k);
    printBase($i, $j + 1, $k);

    printSupport($i, $j, $k);
    printSupport($i + 1, $j, $k);
    printSupport($i + 1, $j, $k + 1);
    printSupport($i, $j, $k + 1);
}

sub printSupport {
    my ($i, $j, $k) = @_;

    printf A "BUILDING %f %f %f    %f %f %f\n",
        $i * $size, $j * $size, $k * $size,
        $i * $size, ($j + 1) * $size, $k * $size;
}

sub printBase {
    my ($i, $j, $k) = @_;
    printf A "BUILDING %f %f %f    %f %f %f\n",
                $i * $size, $j * $size, $k * $size,
                ($i + 1) * $size, $j * $size, $k * $size;
    printf A "BUILDING %f %f %f    %f %f %f\n",
                ($i + 1) * $size, $j * $size, $k * $size,
                ($i + 1) * $size, $j * $size, ($k + 1) * $size;
    printf A "BUILDING %f %f %f    %f %f %f\n",
                ($i + 1) * $size, $j * $size, ($k + 1) * $size,
                ($i) * $size, $j * $size, ($k + 1) * $size;
    printf A "BUILDING %f %f %f    %f %f %f\n",
                ($i) * $size, $j * $size, ($k + 1) * $size,
                ($i) * $size, $j * $size, ($k) * $size;
}
