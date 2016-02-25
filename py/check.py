#!/bin/python

import os
import sqlite3
DB_PATH = '/cfg/etc/ucm_config.db'
MOH_PATH = '/app/asterisk/var/lib/asterisk/moh/'
SOUND_PATH = '/app/asterisk/var/lib/asterisk/sounds/'

dict_moh_exist = {}
dict_moh_db = {}
dict_moh_not_exist = {}

dict_language_exist = {}
dict_language_db = {}
dict_language_not_exist = {}

def func_moh_exist(path):
    cur_list = os.listdir(path)
    for item in cur_list:
        if os.path.isdir(item):
            moh_exist = item[10:]
            dict_moh_exist[moh_exist] = moh_exist

SELECT_MOH = "select moh_name from moh"
def func_moh_db():
    for row in c.execute(SELECT_MOH):
        moh_db_exist = row[0]
        dict_moh_db[moh_db_exist] = moh_db_exist

def func_moh_not_exist():
    for item in dict_moh_db:
        if not dict_moh_exist.has_key(item) and item != 'default':
            dict_moh_not_exist[item] = item

def func_language_exist(path):
    cur_list = os.listdir(path)
    for item in cur_list:
        if os.path.isdir(item):
            dict_language_exist[item] = item

SELECT_LANGUAGE = "select language_id from languages"
def func_language_db():
    for row in c.execute(SELECT_LANGUAGE):
        language_db_exist = row[0]
        dict_language_db[language_db_exist] = language_db_exist

def func_language_not_exist():
    for item in dict_language_db:
        if not dict_language_exist.has_key(item) :
            dict_language_not_exist[item] = item

DELETE_LANGUAGE = "delete from languages where language_id = 'TODELETE'"
DELETE_MOH = "delete from moh where moh_name = 'TODELETE'"
LANGUAGE_SETTINGS_SQL = "select language from language_settings"
LANGUAGE_SETTINGS_UPDATE = "update language_settings set  language = 'en'"

def func_del():
    for item in dict_moh_not_exist:
        DELETE_MOH_SQL = DELETE_MOH.replace("TODELETE", item)
        c.execute(DELETE_MOH_SQL)
    for item in dict_language_not_exist:
        DELETE_LANGUAGE_SQL = DELETE_LANGUAGE.replace("TODELETE", item)
        c.execute(DELETE_LANGUAGE_SQL)
    set_default_lang = ""
    for row in c.execute(LANGUAGE_SETTINGS_SQL):
        lang = row[0]
        if dict_language_not_exist.has_key(lang):
            set_default_lang = 'yes'
    if set_default_lang == 'yes':
        c.execute(LANGUAGE_SETTINGS_UPDATE)
    conn.commit()


conn=sqlite3.connect(DB_PATH)
conn.row_factory = sqlite3.Row
c=conn.cursor()

func_moh_exist(MOH_PATH)
func_moh_db()
func_moh_not_exist()
func_language_exist(SOUND_PATH)
func_language_db()
func_language_not_exist()
func_del()

conn.close()


