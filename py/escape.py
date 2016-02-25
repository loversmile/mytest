#!/usr/bin/env python
# coding=utf-8

def str_escape(varstr, default_str = ""):
    aa = varstr.replace('a','\\a')
    return aa


a = '12qwea'
p = str_escape(a)
print p 
