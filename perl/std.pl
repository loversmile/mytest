#!/usr/bin/perl 

print "What is your name?\n";
$name = <STDIN>;
print "Hello $name";

open(DATA, "<import.txt") or die "Can't open data";
@lines = <DATA>;
close(DATA);
print "@lines";
