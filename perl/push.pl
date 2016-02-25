#!/usr/bin/perl

@coins = ("Quarter","Dime","Nickel");
print "1. \@coins = @coins\n";

push(@coins, "Last");
print "2. \@coins = @coins\n";

unshift(@coins, "First");
print "3. \@coins = @coins\n";

pop (@coins);
print "4. \@coins = @coins\n";

shift(@coins);
print "5. \@coins = @coins\n";

