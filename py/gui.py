#!/usr/bin/env python
# coding=utf-8

import Tkinter

frmMain = Tkinter.Tk()
frmMain.title('Jxx NB')
frmMain.geometry('200x100')
frmMain.resizable(width=False, height=True)
label = Tkinter.Label(frmMain, bg='red', font=('Arial', 12), text='Lou Junkai NB!')
label.pack()

frm = Tkinter.Frame(frmMain)
frm_L = Tkinter.Frame(frm)
Tkinter.Label(frm_L, text='999').pack(side='right')

frmMain.mainloop()
