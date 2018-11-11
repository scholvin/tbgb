#pragma once

#include <mutex>
#include <array>
#include "global.h"

// the internal frame buffer - a matrix of RGB triplets
class Framebuf
{
public:
    struct Data
    {
        double red;
        double green;
        double blue;
    };

private:
    std::array<std::array<Data, TBGB_YMAX>, TBGB_XMAX> m_buf;
    std::mutex m_mutex;

public:
    Framebuf()
    {
        for (int x = 0; x < TBGB_XMAX; x++)
            for (int y = 0; y < TBGB_YMAX; y++)
                m_buf[x][y].red = m_buf[x][y].green = m_buf[x][y].blue = 0;
    }

    Data& data(int x, int y)
    {
        return m_buf[x][y];
    }

    std::mutex& mutex() { return m_mutex; }

    int width() const { return TBGB_XMAX; }
    int height() const { return TBGB_YMAX; }

    // draw a line from (x0,y0) to (x1,y1) in color data
    void line(int x0, int y0, int x1, int y1, Data data);

private:
    static int sign(double x);
    void lineLow(int x0, int y0, int x1, int y1, Data data);
    void lineHigh(int x0, int y0, int x1, int y1, Data data);

};
