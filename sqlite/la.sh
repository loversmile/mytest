#!/bin/sh

for i in $(seq 100); do
#    echo insert or replace into abcde values\($i, \'name\'\)\;  >> la.sql
    RUNSQL = "insert or replace into abcde values('name', %d) " % 
    echo c.execute()
done;
