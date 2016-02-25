#!/usr/bin/env python
# coding=utf-8

def print_args(function):
    def wrapper(*args, **kwargs):
        print "HAHA", args, kwargs
        return function
    return wrapper

@print_args
def ww(pp):
    print pp

ww('nima')


a = [0,1,2,3,4,5]
LAST = slice(-3,None)
print a[LAST]

p = [1,2,3]
q = ['a','b','c']
z = zip(p,q)
print 'z',z
zz = zip(*z)
print "zz",zz


m = {'a':1,'b':2,'c':3}
mi = dict(zip(m.values(),m.keys()))
print m
print mi

md = {v:k for k,v in m.items()}
print md
