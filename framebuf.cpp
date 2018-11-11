#include <cmath>
#include <iostream>
#include "framebuf.h"

// Bresenham rides again
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void
Framebuf::line(int x0, int y0, int x1, int y1, Data data)
{
#if 0
    if (x0 >= TBGB_XMAX || x1 >= TBGB_XMAX || y0 >= TBGB_YMAX || y1 >= TBGB_YMAX ||
        x0 < 0 || x1 < 0 || y0 < 0 || y1 <0)
    {
        std::cout << "invalid coordinate for Framebuf::line (" << x0 << "," << y0 << ") (" << x1 << "," << y1 << ")"<< std::endl;
        return;
    }
#endif
    if (x0 == x1)
    {
        // special case for vertical
        for (int y = y0; y <= y1; ++y) // TODO clip check horiz/vert
            m_buf[x0][y] = data;
        return;
    }
    if (y0 == y1)
    {
        // special case for horizontal
        for (int x = x0; x <= x1; ++x)
            m_buf[x][y0] = data;
        return;
    }

    if (std::abs(y1-y0) < std::abs(x1-x0))
        if (x0 > x1)
            lineLow(x1, y1, x0, y0, data);
        else
            lineLow(x0, y0, x1, y1, data);
    else
        if (y0 > y1)
            lineHigh(x1, y1, x0, y0, data);
        else
            lineHigh(x0, y0, x1, y1, data);
}

void
Framebuf::lineLow(int x0, int y0, int x1, int y1, Data data)
{
    double dx = x1 - x0;
    double dy = y1 - y0;
    int yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    double D = 2*dy - dx;
    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        if (x >= 0 && x < TBGB_XMAX && y >= 0 && y < TBGB_YMAX)
        {
            // silent clip
            m_buf[x][y] = data;
        }
        if (D > 0)
        {
            y += yi;
            D -= 2*dx;
        }
        D += 2*dy;
    }
}

void
Framebuf::lineHigh(int x0, int y0, int x1, int y1, Data data)
{
    double dx = x1 - x0;
    double dy = y1 - y0;
    int xi = 1;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }
    double D = 2*dx - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++)
    {
        if (x >= 0 && x < TBGB_XMAX && y >= 0 && y < TBGB_YMAX)
        {
            // silent clip
            m_buf[x][y] = data;
        }
        if (D > 0)
        {
            x += xi;
            D -= 2*dy;
        }
        D += 2*dx;
    }
}
int
Framebuf::sign(double x)
{
    if (x < 0)
        return -1;
    if (x == 0.0)
        return 0;
    return 1;
}
