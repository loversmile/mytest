#!/usr/bin/python
# Filename: func_param.py

def printMax(a, b):
	if a > b:
		print a, 'is maximum'
	elif a == b:
		print 'The two number is the same'
	else:
		print b, 'is maximum'
printMax(3, 4)
printMax(5, 5)
x = 6
y = 7
printMax(x, y)
