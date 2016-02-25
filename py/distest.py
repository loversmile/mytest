#! /usr/bin/python

import dis

def sum():
	va = 10
	vb = 20
	sum = va + vb
	print "va + vb = %s " % sum

dis.dis(sum)
