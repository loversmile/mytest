#! /usr/bin/python
# Filename: conf.py

import ConfigParser

cf = ConfigParser.ConfigParser()
cf.read("test.conf")

s = cf.sections()
print 'section:', s

o = cf.options("general")
print 'options:', o

v = cf.items("general")
print 'general', v

cf.set("jklou", "love", "xiaoxiao")
cf.write(open("test.conf", "w"))

#cf.add_section("jia")
#cf.set("jia", "age", "25")
#cf.set("jia", "like", "lou")
#cf.write(open("test.conf", "w"))

print cf.get("jklou", "love")

if cf.has_section("jklou"):
	print 'jklou is handsome'
