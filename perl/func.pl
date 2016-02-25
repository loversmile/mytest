#!/usr/bin/perl

sub Average{
    $n = scalar(@_);
    $sum = 0;

    foreach $item(@_){
        $sum += $item;
    }
    $average = $sum/$n;

    print "$average\n"
}

Average(10,20,04);
