#!/usr/bin/env python
# coding=utf-8

import re

a = 'abcdef&g+7GHG989*#'
p = re.compile(r"[^a-zA-Z0-9\*\+#]+")
c = re.search(p,a)
if c:
    print 'match'
else:
    print 'no'
