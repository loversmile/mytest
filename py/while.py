#!/usr/bin/python
# Filename: while.py

number = 23
running = True

while running:
	guess = int(raw_input('Enter an integer:'))

	if guess == number:
		print 'Congratulations, you guessed it.'
		print "(but you do not win any prizes!)"
	elif guess < number:
		print 'NO, it is a little higer than that'
	else:
		print 'NO, it is a little lower than that'
else:
	print 'The while loop is over'

print 'Done'
