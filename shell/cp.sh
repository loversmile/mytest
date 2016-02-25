#! /bin/sh


if diff -r $1 $2;
then
    echo 'loujunkai'
    echo $1
else 
    echo 'jxx'
    cp  $1 $2
fi
