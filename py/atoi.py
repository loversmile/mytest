
import string

if type('2')==type(1):
	print "yes"
else:
	print "no"

def func(x):
	try:
		x=int(x)
		return isinstance(x,int)
	except ValueError:
		return False








if func('1'):
	print "keyi"
if func('1-2'):
	print "1-2"
