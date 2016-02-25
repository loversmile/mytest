#!/usr/bin/python


def scrip_bracket(value):
	res = ''
	for i in range(len(value)):
#		if i != 0 and i != 1:
#			res+=value[i]
		if value[i] == '>' or value[i] == ' ':
			res+=''
		else:
			res+=value[i]
	return res

aaa=scrip_bracket('> abc')
print aaa
