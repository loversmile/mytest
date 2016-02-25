#! /usr/bin/python
# Filename: conf2dbtest.py

import ConfigParser
import sqlite3

#conn = connect("test.db")
cf = ConfigParser.RawConfigParser()
cf.read("test2.conf")

conn=sqlite3.connect("test.db")
conn.row_factory = sqlite3.Row
cu=conn.cursor()

SQL_SELECT='select * from db'

o = cf.options("db")
#print o
#for i in range(len(o)):
#	print o[i]
#	for row in cu.execute(SQL_SELECT):
#		for j in range(len(row.keys())):
#			if o[i]==row.keys()[j]:
#				SQL="update db set "+o[i]+"='"+cf.get('db', o[i])+"' where 'id' <> '10000'"
#				print SQL
#				cu.execute(SQL)

#SQL_INFO='pragma table_info(db)'
cu.execute(SQL_SELECT)
col_name_list = [tuple[0] for tuple in cu.description]  
print col_name_list  

kkk=''
vvv=''
uu=''
for i in range(len(o)):
#for row in cu.execute(SQL_SELECT):
	for j in range(len(col_name_list)):
		if o[i]	== col_name_list[j]:

#	cu.execute(SQL_INFO)

			if i != 0:
				kkk+=","
				vvv+=","
			kkk+="'"+o[i]+"'"
			vvv+="'"+cf.get("db", o[i])+"'"
			
			if i != 0:
				uu+=","
			uu+=o[i]+"='"+cf.get("db", o[i])+"'"

#SQL_DELETE='delete from db'
#SQL_INSERT='insert into db('+kkk+') values('+vvv+')'
SQL_UPDATE='update db set '+uu
print SQL_UPDATE

#cu.execute(SQL_DELETE)
#cu.execute(SQL_INSERT)
cu.execute(SQL_UPDATE)


conn.commit()
conn.close()
