#!/usr/bin/env python
# coding=utf-8

from Tkinter import *
root = Tk()
root.title("hello world")
root.geometry('300x200')

Label(root, text='1111'.decode('gbk').encode('utf8'), font=('Arial', 20)).pack(side=BOTTOM)

frm = Frame(root)
#left
frm_L = Frame(frm)
Label(frm_L, text='0000'.decode('gbk').encode('utf8'), font=('Arial', 15)).pack(side=TOP)
Label(frm_L, text='0101'.decode('gbk').encode('utf8'), font=('Arial', 15)).pack(side=TOP)
frm_L.pack(side=LEFT)

#right
frm_R = Frame(frm)
Label(frm_R, text='8888'.decode('gbk').encode('utf8'), font=('Arial', 15)).pack(side=TOP)
Label(frm_R, text='7878'.decode('gbk').encode('utf8'), font=('Arial', 15)).pack(side=TOP)
frm_R.pack(side=RIGHT)

frm.pack(side=BOTTOM)

root.mainloop()
