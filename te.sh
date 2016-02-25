#!/bin/sh

a=10

while [ $a -le 20 ]
do
   echo $a
   a=`expr $a + 1`
done
