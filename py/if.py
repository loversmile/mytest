#!/usr/bin/python
# Filename: if.py

number = 23
guess = int(raw_input('Enter an integer:'))

if guess == number:
	print 'Congratulations, you guessed it.'
	print "(but you do not win any prizes!)"
elif guess < number:
	print 'NO, it is a little higer than that'
else:
	print 'NO, it is a little lower than that'

print 'Done'



print "IF" if '1'=='2' else  "ELSE"
