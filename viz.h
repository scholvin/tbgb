#pragma once

#include <gtkmm/drawingarea.h>

class Viz : public Gtk::DrawingArea
{
public:
  Viz();
  virtual ~Viz();

protected:
  // Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};
