#include <cairomm/context.h>
#include <iostream>
#include <chrono>
#include <array>
#include "global.h"
#include "viz.h"


namespace { // hi Jeff

// TODO figure all this out on the fly someday
static const int XOFFSET = 36;
static const int YOFFSET = 36;
static const int XSCALE = 36;
static const int YSCALE = 36;
static const int RECTANGLE = 14;

struct P
{
    double x, y;
};

// T (absolute x and y)
const std::vector<P> T = { {-0.5, -0.5}, {8.5, -0.5}, {8.5, 2.5}, {5.5, 2.5},
                           {5.5, 16.5}, {2.5, 16.5}, {2.5, 2.5}, {-0.5, 2.5} };
// outer part of B (relative x, absolute y)
const std::vector<P> Bo = { {0, -0.5}, {7.5, -0.5}, {9, 1}, {9, 7.5},
                            {8.5, 8.0}, {9, 8.5}, {9, 15}, {7.5, 16.5}, {0, 16.5}, };
// inner part of B (relative x and y)
const std::vector<P> Bi = { {3, 0}, {6, 0}, {6, 4}, {3, 4} };
// G (absolute x and y)
const std::vector<P> G = { {21, -0.5}, {28.5, -0.5}, {28.5, 2.5}, {22.5, 2.5},
                           {22.5, 13.5}, {25.5, 13.5}, {25.5, 6.5}, {28.5, 6.5}, {28.5, 16.5},
                           {21, 16.5}, {19.5, 15.0}, {19.5, 1} };

// x dimension is 1 larger to account for null-terminated string
const std::array<std::array<char, TBGB_XMAX+1>, TBGB_YMAX> visible = {
    "XXXXXXXXX XXXXXXXX   XXXXXXXX XXXXXXXX ",
    "XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX",
    "XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX",
    "   XXX    XXX   XXX XXX       XXX   XXX",
    "   XXX    XXX   XXX XXX       XXX   XXX",
    "   XXX    XXX   XXX XXX       XXX   XXX",
    "   XXX    XXX   XXX XXX       XXX   XXX",
    "   XXX    XXXXXXXXX XXX   XXX XXXXXXXXX",
    "   XXX    XXXXXXXX  XXX   XXX XXXXXXXX ",
    "   XXX    XXXXXXXXX XXX   XXX XXXXXXXXX",
    "   XXX    XXX   XXX XXX   XXX XXX   XXX",
    "   XXX    XXX   XXX XXX   XXX XXX   XXX",
    "   XXX    XXX   XXX XXX   XXX XXX   XXX",
    "   XXX    XXX   XXX XXX   XXX XXX   XXX",
    "   XXX    XXXXXXXXX XXXXXXXXX XXXXXXXXX",
    "   XXX    XXXXXXXXX XXXXXXXXX XXXXXXXXX",
    "   XXX    XXXXXXXX   XXXXXXXX XXXXXXXX "
};
}

Viz::Viz(Framebuf& fb) : m_fb(fb)
{
    set_size_request(1430, 650); // TODO constants
}

Viz::~Viz()
{
}

bool
Viz::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    auto start = std::chrono::system_clock::now();

    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    std::cout << "on_draw " << width << "x" << height << std::endl;

    // set gray background
    cr->set_line_width(1);
    cr->set_source_rgb(0.25, 0.25, 0.25);
    cr->rectangle(0, 0, width, height);
    cr->fill();
    cr->stroke();

    // SOMEDAY there is a better way to do this without all the constants, etc
    cr->set_source_rgb(0.5, 0.5, 0.5); // gray
    // draw a T
    for (int i = 0; i < T.size(); ++i)
    {
        const P& p = T[i];
        if (i == 0)
            cr->move_to(xformx(p.x), xformy(p.y));
        else
            cr->line_to(xformx(p.x), xformy(p.y));
    }
    cr->line_to(xformx(T[0].x), xformy(T[0].y));
    cr->stroke();

    // draw a B
    double b = 9.5;
    for (int i = 0; i < Bo.size(); ++i)
    {
        const P& p = Bo[i];
        if (i == 0)
            cr->move_to(xformx(b+p.x), xformy(p.y));
        else
            cr->line_to(xformx(b+p.x), xformy(p.y));
    }
    cr->line_to(xformx(b+Bo[0].x), xformy(Bo[0].y));
    cr->stroke();
    double y0 = 2.5;
    for (int i = 0; i < Bi.size(); ++i)
    {
        const P& p = Bi[i];
        if (i == 0)
            cr->move_to(xformx(b+p.x), xformy(y0+p.y));
        else
            cr->line_to(xformx(b+p.x), xformy(y0+p.y));
    }
    cr->line_to(xformx(b+Bi[0].x), xformy(y0+Bi[0].y));
    cr->stroke();
    y0 = 9.5;
    for (int i = 0; i < Bi.size(); ++i)
    {
        const P& p = Bi[i];
        if (i == 0)
            cr->move_to(xformx(b+p.x), xformy(y0+p.y));
        else
            cr->line_to(xformx(b+p.x), xformy(y0+p.y));
    }
    cr->line_to(xformx(b+Bi[0].x), xformy(y0+Bi[0].y));
    cr->stroke();

    // draw a G
    for (int i = 0; i < G.size(); ++i)
    {
        const P& p = G[i];
        if (i == 0)
            cr->move_to(xformx(p.x), xformy(p.y));
        else
            cr->line_to(xformx(p.x), xformy(p.y));
    }
    cr->line_to(xformx(G[0].x), xformy(G[0].y));
    cr->stroke();

    // draw another B
    b = 29.5;
    for (int i = 0; i < Bo.size(); ++i)
    {
        const P& p = Bo[i];
        if (i == 0)
            cr->move_to(xformx(b+p.x), xformy(p.y));
        else
            cr->line_to(xformx(b+p.x), xformy(p.y));
    }
    cr->line_to(xformx(b+Bo[0].x), xformy(Bo[0].y));
    cr->stroke();
    y0 = 2.5;
    for (int i = 0; i < Bi.size(); ++i)
    {
        const P& p = Bi[i];
        if (i == 0)
            cr->move_to(xformx(b+p.x), xformy(y0+p.y));
        else
            cr->line_to(xformx(b+p.x), xformy(y0+p.y));
    }
    cr->line_to(xformx(b+Bi[0].x), xformy(y0+Bi[0].y));
    cr->stroke();
    y0 = 9.5;
    for (int i = 0; i < Bi.size(); ++i)
    {
        const P& p = Bi[i];
        if (i == 0)
            cr->move_to(xformx(b+p.x), xformy(y0+p.y));
        else
            cr->line_to(xformx(b+p.x), xformy(y0+p.y));
    }
    cr->line_to(xformx(b+Bi[0].x), xformy(y0+Bi[0].y));
    cr->stroke();

    // DRAW THE PIXELS!
    {
        std::lock_guard<std::mutex> lock(m_fb.mutex());
        for (int x = 0; x < TBGB_XMAX; x++)
        {
            for (int y = 0; y < TBGB_YMAX; y++)
            {
                if (visible[y][x] == ' ') // this looks backwards but it's right
                {
                    continue;
                }
                //cr->arc(xformx(x), xformy(y), RADIUS, 0, 2 * M_PI);
                cr->set_source_rgb(m_fb.data(x, y).red, m_fb.data(x, y).green, m_fb.data(x, y).blue);
                cr->rectangle(xformx(x)-RECTANGLE/2, xformy(y)-RECTANGLE/2, RECTANGLE/2, RECTANGLE/2);
                cr->fill_preserve();
                cr->stroke();
            }
        }
    }

    cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);   // green
    cr->move_to(0, 0);
    cr->line_to(width-1, height-1);
    cr->stroke();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "elapsed on_draw time: " << diff.count() << std::endl;
    return true;
}

double
Viz::xformx(double x)
{
    return XOFFSET + XSCALE * x;
}

double
Viz::xformy(double y)
{
    return YOFFSET + YSCALE * y;
}
