#!/usr/local/bin/python

import Tkinter
import viz
import framebuf
import animations

import time

XMAX = 39
YMAX = 17

fb = framebuf.Framebuf(XMAX, YMAX)
root = Tkinter.Tk()
root.winfo_toplevel().title("TBGB")
v = viz.Viz(root)

bframe1 = Tkinter.Frame(root, height=60)
bframe1.grid(row=1)
bframe2 = Tkinter.Frame(root, height=60)
bframe2.grid(row=2)

# TODO: space these better and make them taller
butts = []
for u in range(0, 10):
    b = Tkinter.Button(bframe1, text="animation "+str(u+1))
    b.grid(row=1, column=u)
    butts.append(b)
for u in range(10, 20):
    b = Tkinter.Button(bframe2, text="animation "+str(u+1))
    b.grid(row=2,column=u-10)

def blackout():
    b = animations.Blackout(v, fb, root)
    b.animate()

def whiteout():
    w = animations.Whiteout(v, fb, root)
    w.animate()

def tbgb():
    t = animations.TBGB(v, fb, root)
    t.animate()

butts[0].config(command=blackout, text="blackout")
butts[1].config(command=whiteout, text="whiteout")
butts[2].config(command=tbgb, text="TBGBdemo")
# start with all black
blackout()

# bring the window to the front
root.lift()

root.mainloop()

