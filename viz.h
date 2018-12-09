#pragma once

#include <gtkmm/drawingarea.h>
#include "framebuf.h"

// responsible for rendering a visualization of the framebuf on the screen for debugging
class Viz : public Gtk::DrawingArea
{
public:
    Viz(Framebuf& fb);
    virtual ~Viz();

    // called from outside
    void render();

    void set_animation_name(const std::string& name) { m_animation_name = name; }

protected:
    // Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    static double xformx(double x);
    static double xformy(double y);

private:
    Framebuf& m_fb;
    std::string m_animation_name;
};
