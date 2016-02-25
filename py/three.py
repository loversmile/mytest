#!/usr/bin/env python
# coding=utf-8


a = '[aaa]'
o = "sds"
b = a.replace('[aaa]', "" if o == "" else (':'+o))
print '+++<' +b+ '>+++'
