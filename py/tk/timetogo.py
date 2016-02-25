#!/usr/bin/env python
# coding=utf-8

from Tkinter import *
import tkFont
import os
from functools import partial
from PIL import Image

def clear(entry):
    entry.delete(0, END)

def get_result(year, month, day, result):
    a = str(year) + str(month) + str(day)
    result.set(a)

def get_exit(root):
    root.quit()

def calc(entry):
    input = entry.get()
    output = str(eval(input.strip()))
    clear(entry)
    entry.insert(END, output)

def cal():
    root = Tk()
    root.title('Time Go')
    root.resizable(0, 0)


    year_n = StringVar()
    month_n = StringVar()
    day_n = StringVar()

    entry_font = tkFont.Font(size=12)
    entry_y = Entry(root, width=4, justify="right", font=entry_font,textvariable = year_n)
    entry_y.grid(row=0, column=0, columnspan=1, sticky=W+E, padx=5,  pady=5)

    entry_m = Entry(root, width=4, justify="right", font=entry_font,textvariable = month_n)
    entry_m.grid(row=0, column=2, columnspan=1, sticky=W+E, padx=5,  pady=5)

    entry_d = Entry(root, width=4, justify="right", font=entry_font,textvariable = day_n)
    entry_d.grid(row=0, column=4, columnspan=1, sticky=W+E, padx=5,  pady=5)
    
    Label(root, text='year').grid(row=0, column=1,sticky=W)
    Label(root, text='month').grid(row=0, column=3,sticky=W)
    Label(root, text='day').grid(row=0, column=5,sticky=W)

    result = StringVar()
    Label(root, textvariable = result).grid(row=2,column=0, columnspan = 10, sticky=W+E)

    #button_font = tkFont.Font(size=10, weight=tkFont.BOLD)
    button_bg = '#D5E0EE'
    button_active_bg = '#FFFF22'

    myButton = partial(Button, root, bg=button_bg, padx=10, pady=3, activebackground=button_active_bg)
    submit = myButton(text='Sub', command=lambda : get_result(year_n, month_n,day_n, result))
    submit.grid(row=1, column=0, pady = 5)

    cal = myButton(text="Calc", command=lambda : calc(entry))
    cal.grid(row=1, column=1,pady = 5)

    exit = myButton(text="Exit", command=lambda : get_exit(root));
    exit.grid(row=1, column=2, pady = 5)

    root.mainloop()


if __name__ == '__main__':
    cal()


