#!/bin/bash

for i in {1..100000}
do
    nice -n 0 wget https://192.168.124.126:8089/
done
