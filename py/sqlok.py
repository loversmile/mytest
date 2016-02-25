#! /usr/bin/python
# FIlename: sqlok.py

import sqlite3
import os

DB_FILE_PATH = ''

TABLE_NAME = ''

SHOW_SQL = True



def get_conn(path):
	'''get connect database path'''

	conn = sqlite3.connect(path)
	if os.path.exists(path) and os.path.isfile(path):
		print ('hard disc : [{}].formar(path)')
		return conn
	else:
		conn = None
		print ('memory')
		return sqlite3.connnect(':memory')

def get_cursor(conn):
	'''get cursor of database'''

	if conn is not None:
		return conn.cursor()
	else:
		return get_conn('').cursor()

def drop_table(conn,table):
	'''if table exist, drop table'''
	if table is not None and table != '':
		sql = 'DROP TABLE IF EXISTS' + table
		if SHOW_SQL:
			print('run sql:[{}]'.format(sql))
		cu = get_cursor(conn)
		cu.execute(sql)
		conn.commit()
		print('drop table [{}] success'.format(table))
		close_all(conn,cu)
	else:
		print('the [{}] is empty or equal None'.format(sql))

def create_table(conn,sql):
	'''create tables'''
	if sql is not None and sql != '':
		cu = get_cursor(conn)
		if SHOW_SQL:
			print('run sql [{}]'.format(sql))
		cu.execute(sql)
		conn.commit()
		print('create database table success')
		close_all(conn,cu)
	else:
		print('the [{}] is empty or equal None!'.format(sql)

def close_all(conn,cu):
	'''close database cursor'''
	try:
		if cu is not None:
			cu.close()
	finally:
		if cu is not None:
			cu.close()

def save(conn,sql,data):
	'''insert data'''
	if sql is not None and sql != '':
		if data is not None:
			cu = get_cursor(conn)
			for d in data:
				if SHOW_SQL:
					print('run sql:[{}], :[{}]'.format(sql,d))
				cu.execute(sql,d)
				conn.commit()
			close_all(conn,cu)
	else:
		print('the [{}] is empty or equal None!'.format(sql))

def fetchall(conn,sql):
	'''see about all the data'''
	if sql is not None and sql != '':
		cu = get_cursor(conn)
		if SHOW_SQL:
		   print('run sql: [{}]'.format(sql))
		cu.execute(sql)
		r = cu.fetchall()
		if len(r) > 0:
			for e in range(len(r)):
				print(r[e])
	else:
		print('the [{}] is empty or equal None!'.format(sql)

def fetchone(conn, sql, data)
	'''search one data'''
	if sql is not None and sql != '':
		if data is not None:
			d = (data,)
			cu = get_cursor(conn)
			if SHOW_SQL:
				print('run sql :[{}], : [{}]'.format(sql, data))
			cu.execute(sql, d)
			r = cu.fetchall()
			if len(r) > 0:
				for e in range(len(r)):
					print(r[e])
		else:
			print('the [{}] equal None!'.format(data))
	else:
		print('the [{}] is empty or equal None!'.format(sql))

def update(conn, sql, data)
	'''update data'''
	if sql is not None and sql != '':
		if data is not None:
			cu = get_cursor(conn)
			for d in data:
				if SHOW_SQL:
					print('run sql:[{}],parameter:[{}]'.format(sql, d))
				cu.execute(sql, d)
				conn.commit()
			close_all(conn, cu)
	else:
		print('the [{}] is empty or equal None!'.format(sql))

def delete(conn, sql, data)
	'''delete data'''
	if sql is not None and sql != '':
		if data is not None:
			cu = get_cursor(conn)
			for d in data:
				if SHOW_SQL:
					print('run sql:[{}],parameter:[{}]'.format(sql, d))
				cu.execute(sql, d)
				conn.commit()
			close_all(conn, cu)
	else:
		print('the [{}] is empty or equal None!'.format(sql))


