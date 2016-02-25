#!/usr/bin/env python
# coding=utf-8

import string
f=open('./time1.py', 'wb+')

tmp = ""

for i in range(300):
    tmp += "'" + str(i) + "'"+ "+"

f.write(tmp)
f.close
