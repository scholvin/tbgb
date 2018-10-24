#!/usr/local/bin/python

import Tkinter
import viz
import framebuf

import time

XMAX = 39
YMAX = 17

T1 = range(0, 9)
B2 = range(10, 19)
G3 = range(20, 29)
B4 = range(0, 39)

TBGB_ALL = [T1, B2, G3, B4]

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

def TBGBdemo():
    def T():
        for x in T1:
            for y in range(YMAX):
                fb.set(x, y, [255, 255, 255])
        v.render(fb)

    def B_1():
        for x in B2:
            for y in range(YMAX):
                fb.set(x, y, [255, 255, 255])
        v.render(fb)

    def G():
        for x in G3:
            for y in range(YMAX):
                fb.set(x, y, [255, 255, 255])
        v.render(fb)

    def B_2():
        for x in B4:
            for y in range(YMAX):
                fb.set(x, y, [255, 255, 255])
        v.render(fb)

    def all():
        T()
        B_1()
        G()
        B_2()

    root.after(0, blackout)
    root.after(200, T)
    root.after(400, B_1)
    root.after(600, G)
    root.after(800, B_2)
    delay = 1000
    for i in range(4):
        root.after(delay, blackout)
        delay += 200
        root.after(delay, whiteout)
        delay += 200

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
butts[2].config(command=TBGBdemo, text="TBGBdemo")
# start with all black
blackout()

# bring the window to the front
root.lift()

root.mainloop()

