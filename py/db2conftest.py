#! /usr/bin/python
# Filename: db2conftest.py

import sqlite3
import ConfigParser

SQL_SELECT='select * from catalog'
def cata_db2conftest():
	for row in cu.execute(SQL_SELECT):
		section_name = 'cata'
		if cf.has_section(section_name):
			cf.remove_section(section_name)
		cf.add_section(section_name)

		for i in range(len(row.keys())):
			key = row.keys()[i]
			value = row[i]
			if value != '':
				cf.set(section_name, key, value)

def insert(conn, sql):

	if sql is not None and sql != '':
		cu.execute(sql)
		conn.commit()

################main
conn = sqlite3.connect("test.db")
conn.row_factory = sqlite3.Row
cu = conn.cursor()
cf = ConfigParser.RawConfigParser()

#insert(conn, 'insert into catalog values(3,4,"jia")')
cata_db2conftest()
conn.close()
with open('test.conf', 'wb+') as conffile:
		cf.write(conffile)

