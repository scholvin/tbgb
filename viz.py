import Tkinter

class Viz(object):
    def __init__(self, parent, height, width):
        self.height = height
        self.width = width
        self.canvas = Tkinter.Canvas(parent, height=height, width=width, bg='#303030')
        self.canvas.pack()




