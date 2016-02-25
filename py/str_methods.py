#!/usr/bin/python
# Filename: str_methods.py

name = 'Swaroop'

if name.startswith('Swa'):
	print 'Yes'

returnValue = name.find('war')
print 'return value of find is ', returnValue

delimiter = '_*_*_*_'
mylist = ['monkey', 'chicken', 'dog', 'cat', 'bird']
print delimiter.join(mylist)
