#!/usr/bin/perl 

@foods = qw(a b c d e f g h);
print "Food: @foods\n";

$[ = 2;

print $foods[2],"\n";
