#!/usr/bin/python
# Filename: many.py

import sys, os

def afileline(f_path):	
	res = 0
	f = open(f_path, "r", 1, "utf8")
	for lines in f:
		if(lines.split()):
			res += 1
	return res

if(__name__ == '__main__'):
	host=
