#!/usr/bin/env python
# coding=utf-8
import math

for i in range(2,100):
    flag = 1
    for j in range(2, int(math.sqrt(i)) + 1):
        if (i%j == 0 and i != j):
            flag = 0
            break
    if flag == 1:
        print i
