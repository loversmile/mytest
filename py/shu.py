#!/usr/bin/python
import time
import sys
import os

print 'shu'

os.system('touch abcdef')
cmd='echo ' + sys.argv[1] + '> abcdef'
os.system(cmd)


