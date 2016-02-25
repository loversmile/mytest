#!/bin/bash

AA=./arr.sh

if test -n "$AA" ; then
    echo "no"
else
    echo "yes"
fi


if test '0'='0';then 
    echo '0'
else
    echo '1'
fi

PP=asdf
if [  -n "$PP" ]; then
    echo '9090'
else
    echo '0909'
fi
