#! /usr/bin/python

import thread
import time

def print_time(threadName, delay):
    count = 0
    while count < 5:
        time.sleep(delay)
        count += 1
        print "%s: %s" % (threadName, time.ctime(time.time()))

try:
    thread.start_new_thread(print_time, ("No.111",0.2))
    thread.start_new_thread(print_time, ("No.222",0.4))
except:
    print "Error"

while 1:
    pass
