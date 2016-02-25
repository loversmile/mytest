#!/usr/bin/python
# Filename: class_init.py

class Person:
	def __init__(self, name):
		self.name = name
	def SH(self):
		print 'Hello, the initial name is', self.name
p = Person('THE KING')
p.SH()
