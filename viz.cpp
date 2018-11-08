#include <cairomm/context.h>
#include <iostream>
#include "viz.h"

Viz::Viz()
{
}

Viz::~Viz()
{
}

bool Viz::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
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

    cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);   // green
    cr->move_to(0, 0);
    cr->line_to(width-1, height-1);
    cr->stroke();
    return true;
}
