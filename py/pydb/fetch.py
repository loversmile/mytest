import sqlite3

conn=sqlite3.connect("test.db")
c=conn.cursor()

c.execute("select * from people")

#res = c.fetchall()
res = c.fetchone()
print res
