#include <gtkmm.h>
#include <array>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include "viz.h"
#include "dmx.h"
#include "framebuf.h"
#include "animations.h"
#include "global.h"

class ControlWidget : public Gtk::VBox
{
private:
    Gtk::HBox m_colors;
    Gtk::VScale m_red;
    Gtk::VScale m_green;
    Gtk::VScale m_blue;
    Gtk::VScale m_master;

    Animations& m_anim;

public:
    ControlWidget(Animations& anim) : m_anim(anim)
    {
        m_red.set_range(0, 1);
        m_red.set_inverted(true);
        m_red.set_increments(0.001, 0.1);
        m_red.property_digits() = 3;
        m_red.override_color(Gdk::RGBA("red"));
        m_red.signal_value_changed().connect(sigc::mem_fun(*this, &ControlWidget::on_color_change));

        m_green.set_range(0, 1);
        m_green.set_inverted(true);
        m_green.set_increments(0.001, 0.1);
        m_green.property_digits() = 3;
        m_green.override_color(Gdk::RGBA("green"));
        m_green.signal_value_changed().connect(sigc::mem_fun(*this, &ControlWidget::on_color_change));

        m_blue.set_range(0, 1);
        m_blue.set_inverted(true);
        m_blue.set_increments(0.001, 0.1);
        m_blue.property_digits() = 3;
        m_blue.override_color(Gdk::RGBA("blue"));
        m_blue.signal_value_changed().connect(sigc::mem_fun(*this, &ControlWidget::on_color_change));

        m_colors.add(m_red);
        m_colors.add(m_green);
        m_colors.add(m_blue);
        add(m_colors);

        m_master.set_range(0, 1);
        m_master.set_inverted(true);
        m_master.set_increments(0.001, 0.1);
        m_master.property_digits() = 3;
        m_master.set_value_pos(Gtk::POS_BOTTOM);
        m_master.set_value(1.0);
        m_master.signal_value_changed().connect(sigc::mem_fun(*this, &ControlWidget::on_master_change));
        add(m_master);
        show_all_children();
    }

    void set_color(const Framebuf::Color& color)
    {
        m_red.set_value(color.get_red());
        m_green.set_value(color.get_green());
        m_blue.set_value(color.get_blue());
    }

    Framebuf::Color get_color() const
    {
        Framebuf::Color ret;
        ret.set_red(m_red.get_value());
        ret.set_green(m_green.get_value());
        ret.set_blue(m_blue.get_value());
        return ret;
    }

    void enable()
    {
        m_red.set_sensitive(true);
        m_green.set_sensitive(true);
        m_blue.set_sensitive(true);
    }

    void disable()
    {
        m_red.set_sensitive(false);
        m_green.set_sensitive(false);
        m_blue.set_sensitive(false);
    }

private:
    void on_color_change()
    {
        //std::cout << "red=" << m_red.get_value() << " green=" << m_green.get_value() << " blue=" << m_blue.get_value() << std::endl;
        m_anim.set_global_colors(m_red.get_value(), m_green.get_value(), m_blue.get_value());
    }

    void on_master_change()
    {
        m_anim.set_master(m_master.get_value());
    }
};

class MainWindow : public Gtk::ApplicationWindow
{
public:
    MainWindow(Viz& viz, Animations& animations) : m_viz(viz), m_animations(animations), m_control(animations),
        m_current(nullptr), m_running(true), m_done(false)
    {
        set_title("TBGB");

        add(m_vbox);
        m_vbox.set_homogeneous(false);

        m_topbox.pack_start(m_viz, true, true);
        m_topbox.pack_end(m_control, true, true);

        m_vbox.pack_start(m_topbox, true, true);
        m_vbox.pack_end(m_grid, false, false);
        m_grid.set_hexpand(true);

        auto it = animations.get_all().begin();
        for (auto i = 0; i < 20; i++)
        {
            m_buttons[i].set_hexpand(true);
            if (i < animations.get_all().size())
            {
                m_buttons[i].set_label(std::get<0>(*it));
                m_buttons[i].signal_clicked().connect(
                    sigc::bind<std::string, Animations::AnimFuncType>(sigc::mem_fun(this, &MainWindow::change_animation), 
                                                                      std::get<0>(*it), std::get<1>(*it), std::get<2>(*it)));
            }
            else
            {
                m_buttons[i].set_label("animation " + std::to_string(i+1));
                m_buttons[i].signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(this, &MainWindow::not_impl), i+1));
            }
            m_grid.attach(m_buttons[i], i % 10, i / 10, 1, 1);
            m_buttons[i].get_child()->override_font(Pango::FontDescription("sans bold 18"));
            it++;
        }

        show_all();
        m_thread = std::thread(std::bind(&MainWindow::animation_runner, this));
        m_thread.detach();
    }

    void change_animation(const std::string& name, Animations::AnimFuncType animation, const Framebuf::Color* color)
    {
        std::cout << "change_animation to " << name << std::endl;
        if (color)
        {
            m_control.enable();
            m_control.set_color(*color);
        }
        else
        {
            m_control.disable();
        }
        m_current = animation;
        m_animations.cancel();
    }

    void animation_runner(void)
    {
        std::cout << "animation_runner start" << std::endl;
        while (m_running)
        {
            if (m_current)
            {
                m_current();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        m_done = true;
        std::cout << "animation_runner exit" << std::endl;
    }

    void not_impl(int i)
    {
        std::cout << "animation " << i << " not implemented yet" << std::endl;
    }

    virtual ~MainWindow() 
    {
        std::cout << "main window closing..." << std::endl;
        m_running = false;
        while (!m_done)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

private:
    Viz& m_viz;
    Animations& m_animations;
   
    // current animation
    Animations::AnimFuncType m_current;

    // parallelism
    std::thread m_thread;
    std::atomic_bool m_running;
    std::atomic_bool m_done;

    /*

        Widget layout goes like this (not to scale):

    +----------------------- MainWindow ----------------------------+
    |+------------------------ m_vbox -----------------------------+|
    ||+--------------------- m_topbox ----------------------------+||
    |||+----------- m_viz --------------------++--- m_control ---+|||
    ||||                                      ||                 ||||
    |||+--------------------------------------++-----------------+|||
    ||+-----------------------------------------------------------+||
    ||+----------------------- m_grid ----------------------------+||
    |||                                                           |||
    |||                                                           |||
    ||+-----------------------------------------------------------+||
    +---------------------------------------------------------------+

    */

    // Child widgets:
    Gtk::VBox m_vbox;       // top level, has viz/controls on top, button grid on bottom
    Gtk::HBox m_topbox;     // has viz on left, controls to right
    ControlWidget m_control;
    Gtk::Grid m_grid;
    std::array<Gtk::Button, 20> m_buttons;
};

int main(int argc, char *argv[])
{
    // TODO add menu to quit
    auto app = Gtk::Application::create(argc, argv, "com.tinybitofgiantsblood.tbgb");

    Framebuf fb;
    Viz viz(fb);
    DMX dmx(fb);
    Animations anim(fb, std::bind(&Viz::render, &viz), std::bind(&DMX::render, &dmx));
    MainWindow window(viz, anim);

    auto ret = app->run(window);
    std::cout << "exiting with code " << ret << std::endl;
    return ret;
}
