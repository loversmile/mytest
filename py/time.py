#!/usr/bin/env python
# coding=utf-8

import datetime

day1 = datetime.datetime(2010, 8, 21)

day_now = datetime.datetime.now()

day2= datetime.datetime(2017, 4, 12)

print day_now - day1

print day2 - day1

print day1+datetime.timedelta(days=2200)

print day1.ctime()
