#!/usr/bin/env python
# coding=utf-8


import string
a= 'abcdefgh'
alist = list(a)
b = a.find("c")
print b

alist[2]= ';'#alist[2].upper()

print "".join(alist)
