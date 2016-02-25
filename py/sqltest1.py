#!/usr/bin/python
# Filename: sqltest1.py

import sqlite3

def sqlTest1():
	cx = sqlite3.connect('test.db')
	cu = cx.cursor()
#create
	cu.execute('''create table catalog(
				id integer primary key,
				pid integer,
				name varchar(10) unique
				)''')
if __name__ == '__main__':
	sqlTest1()

