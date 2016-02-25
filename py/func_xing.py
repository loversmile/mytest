#!/usr/bin/env python
# coding=utf-8

def func(a, b, *c):
    print a
    print b
    print c

def func2(a, **d):
    print a
    print d

func(1,2,3,4,5,6)
func2(1, d='0', b='9')
