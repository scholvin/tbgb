import viz
import framebuf
import datetime

T1 = range(0, 9)
B2 = range(10, 19)
G3 = range(20, 29)
B4 = range(0, 39)

TBGB_ALL = [T1, B2, G3, B4]

class AnimationBase(object):
    def __init__(self, v, fb, root):
        self.v = v
        self.fb = fb
        self.root = root

    def render(self):
        self.v.render(self.fb)
        # put dmx renderer here

    def blackout(self):
        for x in range(0, self.fb.width()):
            for y in range(0, self.fb.height()):
                self.fb.set(x, y, [0, 0, 0])

    def whiteout(self):
        for x in range(0, self.fb.width()):
            for y in range(0, self.fb.height()):
                self.fb.set(x, y, [255, 255, 255])

    def log(self, msg):
        print datetime.datetime.utcnow(), msg

class Blackout(AnimationBase):
    def __init__(self, viz, fb, root):
        super(Blackout, self).__init__(viz, fb, root)

    def animate(self):
        self.blackout()
        super(Blackout, self).render()

class Whiteout(AnimationBase):
    def __init__(self, viz, fb, root):
        super(Whiteout, self).__init__(viz, fb, root)

    def animate(self):
        self.blackout()
        super(Whiteout, self).render()

class TBGB(AnimationBase):
    def __init__(self, viz, fb, root):
        super(TBGB, self).__init__(viz, fb, root)

    def T(self):
        for x in T1:
            for y in range(self.fb.height()):
                self.fb.set(x, y, [255, 255, 255])
        super(TBGB, self).render()

    def B_1(self):
        for x in B2:
            for y in range(self.fb.height()):
                self.fb.set(x, y, [255, 255, 255])
        super(TBGB, self).render()

    def G(self):
        for x in G3:
            for y in range(self.fb.height()):
                self.fb.set(x, y, [255, 255, 255])
        super(TBGB, self).render()

    def B_2(self):
        for x in B4:
            for y in range(self.fb.height()):
                self.fb.set(x, y, [255, 255, 255])
        super(TBGB, self).render()

    def animate(self):
        self.root.after(0, self.blackout)
        self.root.after(200, self.T)
        self.root.after(400, self.B_1)
        self.root.after(600, self.G)
        self.root.after(800, self.B_2)
        delay = 1000
        for i in range(4):
            self.root.after(delay, self.blackout)
            self.root.after(delay+1, super(TBGB, self).render())
            delay += 200
            self.root.after(delay, self.whiteout)
            self.root.after(delay+1, super(TBGB, self).render())
            delay += 200
