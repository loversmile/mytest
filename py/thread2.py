#!/usr/bin/env python
# coding=utf-8

import threading
import time
mylock = threading.RLock()
num = 0

class myThread(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.t_name = name

    def run(self):
        global num
        while True:
            mylock.acquire()
            print 'Thread %s locked, Number is : %d' % (self.t_name, num)
            if num >= 10:
                mylock.release()
                print 'Thread %s released, Number is : %d' % (self.t_name, num)
                break
            num += 1
            print 'Thread %s released, Number is : %d' % (self.t_name, num)
            mylock.release()
            time.sleep(1)

def test():
    thread1 = myThread('A')
    thread2 = myThread('B')
    thread1.start()
    thread2.start()

if __name__ == '__main__':
    test()
