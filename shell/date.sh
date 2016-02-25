#!/bin/sh

today=`date +%s`;
tmp=$((daysAgo+1));
sec=$((today-24*3600*tmp));
startDate=`date +%Y-%m-%d -d @$sec`;

echo $startDate

