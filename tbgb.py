#!/usr/local/bin/python

import Tkinter
import viz
import framebuf

XMAX = 39
YMAX = 17

fb = framebuf.Framebuf(XMAX, YMAX)
root = Tkinter.Tk()
root.winfo_toplevel().title("TBGB")
v = viz.Viz(root)

def blackout():
    for x in range(0, XMAX):
        for y in range(0, YMAX):
            fb.set(x, y, [0, 0, 0])
    v.render(fb)

def whiteout():
    for x in range(0, XMAX):
        for y in range(0, YMAX):
            fb.set(x, y, [255, 255, 255])
    v.render(fb)

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

butts[0].config(command=blackout, text="blackout")
butts[1].config(command=whiteout, text="whiteout")

# start with all black
blackout()

# bring the window to the front
root.lift()

root.mainloop()

