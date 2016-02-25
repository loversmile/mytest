#!/usr/bin/env python
# coding=utf-8

import os
import sys

dict = {}
input = open('nvram.cfg', 'r')
num = 0
for line in input:
    a = line.split('=')[0]
    if dict.has_key(a):
        print a
    dict[a] = a
    num += 1
print num
