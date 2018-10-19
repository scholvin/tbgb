#!/usr/local/bin/python

import Tkinter
import viz

root = Tkinter.Tk()
v = viz.Viz(root, width=1000, height=600)
w = Tkinter.Label(root, text="hello world")
w.pack()
root.mainloop()

