

import sqlite3

def run(var):
    RUNSQL = "insert or replace into abcde values('name', '%s') " % var
    c.execute(RUNSQL)


conn=sqlite3.connect('./te.db')
c=conn.cursor()

#c.execute('BEGIN')
c.execute('delete from abcde')

for i in range(1,100):
    run(i)

conn.commit()
conn.close()
