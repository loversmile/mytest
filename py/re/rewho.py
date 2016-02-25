#!/usr/bin/env python
# coding=utf-8

import re
f = open("whodata.txt", 'r')
for eachline in f.readlines():
    print re.split("\s\s+", eachline)
f.close()
