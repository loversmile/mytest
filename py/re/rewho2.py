#!/usr/bin/env python
# coding=utf-8

import re
import os
f = os.popen("who", 'r')
for eachline in f.readlines():
    print re.split("\s\s+", eachline.strip())
f.close()
