#! /usr/bin/python

import Tkinter

def HowTo():
    pass
def About():
    pass

def file_menu():
    file_btn = Tkinter.Menubutton(menu_frame, text = 'Help', underline=0)
    return file_btn

def action_menu():
    action_btn = Tkinter.Menubutton(menu_frame, text = 'Help', underline=0)
    return action_btn

def help_menu():
    help_btn = Tkinter.Menubutton(menu_frame, text = 'Help', underline=0)
    help_btn.pack(side=Tkinter.LEFT, padx="2m")
    help_btn.menu = Tkinter.Menu(help_btn)
    help_btn.menu.add_command(label="How To", underline=0, command=HowTo)
    help_btn.menu.add_command(label="About", underline=0, command=About)
    help_btn['menu'] = help_btn.menu
    return help_btn


top = Tkinter.Tk()
menu_frame = Tkinter.Frame(top)
menu_frame.pack(fill=Tkinter.X, side=Tkinter.TOP)
menu_frame.tk_menuBar(file_menu(), action_menu(), help_menu())
top.mainloop()
