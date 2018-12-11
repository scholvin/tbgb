#pragma once

#include <mutex>
#include <array>
#include <gtkmm.h>
#include "global.h"

// the internal frame buffer - a matrix of RGB triplets
class Framebuf
{
public:
    typedef Gdk::RGBA Color;

    static const Color INCANDESCENT;

    static const Color BLACK;
    static const Color WHITE;
    static const Color RED;
    static const Color ORANGE;
    static const Color YELLOW;
    static const Color GREEN;
    static const Color BLUE;
    static const Color PURPLE;

    static const Color DIM;
    static const Color GREG;

    static const Color RAINBOW[];
    static const int RAINBOW_LENGTH;

private:
    std::array<std::array<Color, TBGB_YMAX>, TBGB_XMAX> m_buf;
    std::mutex m_mutex;

public:
    Framebuf();
    

    Color& data(int x, int y)
    {
        if (x >=0 && x < TBGB_XMAX && y >= 0 && y < TBGB_YMAX)
            return m_buf[x][y];
        else
        {
            s_junk = BLACK;
            return s_junk;
        }
    }

    std::mutex& mutex() { return m_mutex; }

    int width() const { return TBGB_XMAX; }
    int height() const { return TBGB_YMAX; }

    // draw a line from (x0,y0) to (x1,y1) in color data
    void line(int x0, int y0, int x1, int y1, Color color);

private:
    static int sign(double x);
    void lineLow(int x0, int y0, int x1, int y1, Color color);
    void lineHigh(int x0, int y0, int x1, int y1, Color color);

    static Color s_junk;

};
