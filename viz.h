#pragma once

#include <gtkmm/drawingarea.h>
#include "framebuf.h"

// responsible for rendering a visualization of the framebuf on the screen for debugging
// TODO pass the framebuf in at ctor
// TODO lock it during on_draw
class Viz : public Gtk::DrawingArea
{
public:
    Viz(Framebuf& fb);
    virtual ~Viz();

protected:
    // Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    static double xformx(double x);
    static double xformy(double y);

private:
    Framebuf& m_fb;
};
