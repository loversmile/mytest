#!/usr/bin/perl

print ("jelll  ashdjfhasjk\n");

$a=10;
$var = <<"EOF";
This is the syntax for here document and it will continue
until it encounters a EOF in the first line.
This is case of double quote so variable value will be
interpolated. For example value of a = $a
EOF
print "$var\n";

$foo=v48.65.97;
print "$foo\n";
print v49.66.98,"\n";
print __FILE__,"\n";
print __LINE__,"\n";
print __PACKAGE__,"\n";

print "---------------------\n";
@days = qw/Monday Tuesday Sunday/;
print "$days[2]\n";

@var_10 = (1..10);
print "@var_10\n";
print "@days\n";
print "size",scalar @days, "\n";
print $#days,"\n";
