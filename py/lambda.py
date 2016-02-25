#!/usr/bin/python
# Filename: lambda.py

def make_repeater(n):
	return lambda s: s*n

third = make_repeater(3)
print third('lou')
print third(5)
