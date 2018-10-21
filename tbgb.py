#!/usr/local/bin/python

import Tkinter
import viz

# LEDs = 39 x 17

root = Tkinter.Tk()
v = viz.Viz(root)
w = Tkinter.Label(root, text="hello world")
for i in range(0, 39):
    for j in range(0, 17):
        if i % 10 == 0:
            v.plot(i, j, 255, 255, 255)
        else:
            v.plot(i, j, 255, 0, 255)
w.pack()
root.mainloop()

