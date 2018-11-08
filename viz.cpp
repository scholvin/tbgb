#include <cairomm/context.h>
#include <iostream>
#include <chrono>
#include "viz.h"

namespace {
// hi Jeff
static const int XOFFSET = 36;
static const int YOFFSET = 36;
static const int XSCALE = 36;
static const int YSCALE = 36;

struct P
{
    double x, y;
};

// T
const std::vector<P> T = { {-0.5, -0.5}, {8.5, -0.5}, {8.5, 2.5}, {5.5, 2.5},
                           {5.5, 16.5}, {2.5, 16.5}, {2.5, 2.5}, {-0.5, 2.5} };
// outer part of B
const std::vector<P> Bo = { {0, -0.5}, {7.5, -0.5}, {9, 1}, {9, 7.5},
                            {8.5, 8.0}, {9, 8.5}, {9, 15}, {7.5, 16.5}, {0, 16.5}, };
// inner part of B
const std::vector<P> Bi = { {3, 0}, {6, 0}, {6, 4}, {3, 4} };
// G
const std::vector<P> G = { {21, -0.5}, {28.5, -0.5}, {28.5, 2.5}, {22.5, 2.5},
                           {22.5, 13.5}, {25.5, 13.5}, {25.5, 6.5}, {28.5, 6.5}, {28.5, 16.5},
                           {21, 16.5}, {19.5, 15.0}, {19.5, 1} };
}

Viz::Viz(int width, int height)
{
    set_size_request(width, height);
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

    // set black background
    cr->set_line_width(1);
    cr->set_source_rgb(0, 0, 0);
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