#!/usr/bin/env python
# coding=utf-8

import Tkinter
import tkFont
import os
from functools import partial
from PIL import Image

def ma():
    root = Tkinter.Tk()
    root.title("Frame")
    root.resizable(200,200)
    main_frame = Tkinter.Frame(root)
    main_frame.pack(fill=Tkinter.X, side=Tkinter.TOP)
    f_year = Tkinter.Frame(main_frame, relief=Tkinter.RAISED, borderwidth=1)
    f_year.pack(side=Tkinter.TOP, padx=2, pady=1)
    Tkinter.Label(f_year, text="ppppp", width=5).pack(side=Tkinter.LEFT)
    Tkinter.Label(f_year, text="QQQQQ", width=5).pack(side=Tkinter.LEFT)


    root.mainloop()


if __name__ == '__main__':
    ma()
