#!/usr/bin/env python
# coding=utf-8

from ctypes import *
libmath = CDLL("./math_jkl.so")

a = 69;
b = 3;

pp = libmath.plus(a, b)
mi = libmath.minus(a, b)
mu = libmath.multiply(a, b)
di = libmath.div(a, b)

print pp,mi,mu,di
