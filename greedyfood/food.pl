#!/usr/bin/perl -w

# you have three critics defined by (sweetness, saltiness, spicyness)
# you have a list of ingredients which have modifiers for each thing
# you have N turns to add ingredients before your dish is done
# you need to add an ingredient on each turn
# you dish has default (sweetness, saltiness, spicyness) which you need to
#     modify

# input format:
#     (critic 1)
#     (critic 2)
#     (critic 3)
#     TURNS (dish)
#     Q (ingredient)
#       ...

my @numbers = ();

while(<STDIN>) {
    my $s = $_;
    $s =~ s/[\r\n]//g;
    push @numbers, split(/  */, $s);
}

my @critics = ();
for(my $i = 0; $i < 3 * 3; ++$i) {
    push @critics, shift @numbers;
}

my @dish = ();
for(my $i = 0; $i < 4; ++$i) {
    push @dish, shift @numbers;
}

my @ingredients = ();
my $ingrNum = 0;
while(scalar(@numbers) > 0) {
    $ingrNum++;
    push @ingredients, shift @numbers;
}
$ingrNum /= 4;

print "critics: \n";
for(my $i = 0; $i < 3; ++$i) {
    print " ";
    for(my $j = 0; $j < 3; ++$j) {
        print $critics[$i*3 + $j], " ";
    }
    print "\n";
}

print "dish:\n ", $dish[0], "T ", $dish[1], " ", $dish[2], " ", $dish[3], "\n";

print "ingredients:\n";
for(my $i = 0; $i < $ingrNum; ++$i) {
    print " ", $ingredients[$i * 4 + 0], "x ", $ingredients[$i * 4 + 1], " ", $ingredients[$i * 4 + 2], " ", $ingredients[$i * 4 + 3], "\n";
}

# build cost vector for ingredients that are still left
# pick the lowest score
while($dish[0]-- > 0) {
    my $minCost = 32768**2;
    my $minIndex = -1;
    my @minDish = ();
    for(my $i = 0; $i < $ingrNum; ++$i) {
        next if $ingredients[$i* 4] <= 0;
        my @newDish = (
                $dish[1] + $ingredients[$i* 4 + 1],
                $dish[2] + $ingredients[$i* 4 + 2],
                $dish[3] + $ingredients[$i* 4 + 3]);
        my $cost = 0;
        for(my $j = 0; $j < 3; ++$j) {
            $cost += ($newDish[0] - $critics[$j * 3 + 0])**2 +
                ($newDish[1] - $critics[$j * 3 + 1])**2 +
                ($newDish[2] - $critics[$j * 3 + 2])**2;
        }
        if($cost < $minCost) {
            $minCost = $cost;
            $minIndex = $i;
            @minDish = @newDish;
        }
    }
    if($minIndex < 0) {
        print "bad input\n";
        exit 1;
    }
    $ingredients[$minIndex * 4]--;
    $dish[1] = $minDish[0];
    $dish[2] = $minDish[1];
    $dish[3] = $minDish[2];
    print "adding $minIndex with properties ", $ingredients[$minIndex * 4 + 1], " ", $ingredients[$minIndex * 4 + 2], " ", $ingredients[$minIndex * 4 + 3], ", ", $ingredients[$minIndex * 4], " stock left\n";
    print $dish[0], " turns left...\n";
}

print "final dish: ", $dish[1], " ", $dish[2], " ", $dish[3], "\n";
print "\nfinal scores:\n";
my $median = 0;
my @costs = ();
for(my $j = 0; $j < 3; ++$j) {
    print "ciritic ", $j + 1, ": ";
    my $cost = sqrt(($dish[1] - $critics[$j * 3 + 0])**2 +
        ($dish[2] - $critics[$j * 3 + 1])**2 +
        ($dish[3] - $critics[$j * 3 + 2])**2);
    printf "-%.2f\n", $cost;
    push @costs, $cost;
    $median += $cost / 3.0;
}
printf "med: -%.2f\n", $median;
my $sfinal = 0;
for(my $i = 0; $i < 3; ++$i) {
    $sfinal += ($costs[$i] - $median)**2;
}
printf "sigma: %.2f\n", $sfinal;

print "--------- experiments ----------\n";
my @medCritic = (
        (($critics[0] + $critics[3] + $critics[6]) / 3),
        (($critics[1] + $critics[4] + $critics[7]) / 3),
        (($critics[2] + $critics[5] + $critics[8]) / 3));
print "averaged out critic: ";
for my $i (@medCritic) { printf "%.2f ", $i; }
print "\n";
my $medCriticAvg = sqrt(($dish[1] - $medCritic[0])**2 +
        ($dish[2] - $medCritic[1])**2 +
        ($dish[3] - $medCritic[2])**2);
printf "dampened average: %.2f\n", -$medCriticAvg;
printf "divergence from this?: %.2f\n", abs($median - $medCriticAvg) / ($median + $medCriticAvg);
#printf "divergence from this?: %.2f\n", (1 - $medCriticAvg / $median) ** 2;
