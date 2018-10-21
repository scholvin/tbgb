import Tkinter

class Viz(object):
    def __init__(self, parent):
        self.height = 650
        self.width = 1600
        self.xoffset = 36
        self.yoffset = 36
        self.xscale = 36
        self.yscale = 36
        self.radius = 6
        self.canvas = Tkinter.Canvas(parent, height=self.height, width=self.width, bg='#303030')
        self.canvas.pack()

        # draw the T
        self.canvas.create_line(self.xform(-0.5, -0.5), self.xform(8.5, -0.5), self.xform(8.5, 2.5),  self.xform(5.5, 2.5), \
            self.xform(5.5, 16.5), self.xform(2.5, 16.5), self.xform(2.5, 2.5), self.xform(-0.5, 2.5), self.xform(-0.5, -0.5), \
            fill='white')

        # draw a B
        b = 9.5
        self.canvas.create_line(self.xform(b, -0.5), self.xform(b+7.5, -0.5), self.xform(b+9, 1), self.xform(b+9, 7.5), \
            self.xform(b+8.5, 8.0), self.xform(b+9, 8.5), self.xform(b+9, 15), self.xform(b+7.5, 16.5), self.xform(b, 16.5), \
            self.xform(b, -0.5),\
            fill='white')
        y0 = 2.5
        self.canvas.create_line(self.xform(b+3, y0), self.xform(b+6, y0), self.xform(b+6, y0+4), self.xform(b+3, y0+4), \
            self.xform(b+3, y0), fill='white')
        y0 = y0 + 7
        self.canvas.create_line(self.xform(b+3, y0), self.xform(b+6, y0), self.xform(b+6, y0+4), self.xform(b+3, y0+4), \
            self.xform(b+3, y0), fill='white')

        # draw the G
        self.canvas.create_line(self.xform(21, -0.5), self.xform(28.5, -0.5), self.xform(28.5, 2.5), self.xform(22.5, 2.5), \
            self.xform(22.5, 13.5), self.xform(25.5, 13.5), self.xform(25.5, 6.5), self.xform(28.5, 6.5), self.xform(28.5, 16.5), \
            self.xform(21, 16.5), self.xform(19.5, 15.0), self.xform(19.5, 1), self.xform(21, -0.5), fill='white')


        # draw another B
        b = 29.5
        self.canvas.create_line(self.xform(b, -0.5), self.xform(b+7.5, -0.5), self.xform(b+9, 1), self.xform(b+9, 7.5), \
            self.xform(b+8.5, 8.0), self.xform(b+9, 8.5), self.xform(b+9, 15), self.xform(b+7.5, 16.5), self.xform(b, 16.5), \
            self.xform(b, -0.5),\
            fill='white')
        y0 = 2.5
        self.canvas.create_line(self.xform(b+3, y0), self.xform(b+6, y0), self.xform(b+6, y0+4), self.xform(b+3, y0+4), \
            self.xform(b+3, y0), fill='white')
        y0 = y0 + 7
        self.canvas.create_line(self.xform(b+3, y0), self.xform(b+6, y0), self.xform(b+6, y0+4), self.xform(b+3, y0+4), \
            self.xform(b+3, y0), fill='white')

    def xform(self, x, y):
        """map pixel in LED space to canvas coordinates"""
        cx = self.xoffset + self.xscale * x
        cy = self.yoffset + self.yscale * y
        return cx, cy

    def rgb2color(self, red, green, blue):
        """convert (r, g, b) into a # string"""
        hexcode = "#%02x%02x%02x" % (red, green, blue)
        return hexcode

    def plot(self, x, y, red, green, blue):
        """plot the LED pixel at x,y (pixel space) with color r,g,b"""
        cx, cy = self.xform(x, y)
        r = self.radius
        self.canvas.create_oval(cx-r, cy-r, cx+r, cy+r, fill=self.rgb2color(red, green, blue))




