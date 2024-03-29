class Framebuf(object):
    RED = 0
    GREEN = 1
    BLUE = 2
    COLORS = [RED, GREEN, BLUE]

    def __init__(self, width, height):
        self.WIDTH = width
        self.HEIGHT = height

        # set up the framebuf, a 3-dimensional array in [X, Y, RGB], and initialize to black (0's)
        # https://stackoverflow.com/questions/15448594/how-to-add-elements-to-3-dimensional-array-in-python
        self.buf = []
        for x in range(0, self.WIDTH):
            self.buf.append([])
            for y in range(0, self.HEIGHT):
                self.buf[x].append([])
                for counter, z in enumerate([self.RED, self.GREEN, self.BLUE]):
                    self.buf[x][y].append(0)

    def get(self, x, y):
        """return an R G B triple of the pixel at x,y"""
        return self.buf[x][y][self.RED], self.buf[x][y][self.GREEN], self.buf[x][y][self.BLUE]

    def set(self, x, y, triple):
        """set the pixel at x, y to the R G B triple"""
        for counter, z in enumerate(self.COLORS):
            self.buf[x][y][z] = triple[z]

    def width(self):
        return self.WIDTH

    def height(self):
        return self.HEIGHT
