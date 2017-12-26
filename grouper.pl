#!/usr/bin/perl -w

use strict;
use MP3::Tag;
use MP3::Tag::ID3v2;
use autodie;

my $sourcedir = `readlink -f $ARGV[0]`;
chomp $sourcedir;
print "$sourcedir\n";
opendir (A, $sourcedir);

my %artists = ();

foreach my $file (readdir A)
{
    next if(! -f $file);
    my $mp3 = MP3::Tag->new($file);
    $mp3->get_tags();
    my $id3v2 = $mp3->{ID3v2} if exists $mp3->{ID3v2};
    next if !defined($id3v2);

    my ($artist, $album) = (
        $id3v2->frame_select_by_descr("TPE1"),
        $id3v2->frame_select_by_descr("TALB")
    );

    if(!defined($artists{$artist})) {
        $artists{$artist} = {};
    }
    if(!defined($artists{$artist}->{$album} )) {
        $artists{$artist}->{$album} = [];
    }
    push @{$artists{$artist}->{$album}}, $file;
}

use Data::Dumper;
print Dumper(\%artists);

foreach my $artist (keys %artists) {
    `mkdir -p "$sourcedir/$artist"`;
    foreach my $album (keys %{$artists{$artist}}) {
        `mkdir -p "$sourcedir/$artist/$album"`;
        foreach my $file (@{$artists{$artist}->{$album}}) {
            system("mv \"$file\" \"$sourcedir/$artist/$album/\"") == 0 or die('nope');
        }
    }
}
