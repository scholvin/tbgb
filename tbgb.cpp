#include <gtkmm.h>
#include <array>
#include <vector>
#include <iostream>
#include "viz.h"
#include "framebuf.h"
#include "animations.h"
#include "global.h"

void change_animation(Animations::FuncType animation)
{
    std::cout << "new animation " << &animation << std::endl;
}

void not_impl(int i)
{
    std::cout << "animation " << i << " not iumplemented yet" << std::endl;
}

class MainWindow : public Gtk::Window
{
public:
    MainWindow(Viz& viz, const Animations::AnimationList& animations) : m_viz(viz)
    {
        //set_default_size(1430, 650);
        set_title("TBGB");
        add(m_vbox);
        m_vbox.set_homogeneous(false);
        m_vbox.pack_start(m_viz, true, true);
        m_vbox.pack_end(m_grid, false, false);
        m_grid.set_hexpand(true);

        for (auto i = 0; i < 20; ++i)
        {
            // TODO bigger font on label text
            m_buttons[i].set_hexpand(true);
            if (i < animations.size())
            {
                m_buttons[i].set_label(animations[i].first);
                m_buttons[i].signal_clicked().connect(sigc::bind<Animations::FuncType>(sigc::ptr_fun(&change_animation), animations[i].second));
            }
            else
            {
                m_buttons[i].set_label("animation " + std::to_string(i+1));
                m_buttons[i].signal_clicked().connect(sigc::bind<int>(sigc::ptr_fun(&not_impl), i+1));
            }
            m_grid.attach(m_buttons[i], i % 10, i / 10, 1, 1);
        }

        show_all();
    }

    virtual ~MainWindow() { }

private:
    Viz& m_viz;

    // Child widgets:
    Gtk::VBox m_vbox;
    Gtk::Grid m_grid;
    std::array<Gtk::Button, 20> m_buttons;
};

int main(int argc, char *argv[])
{
    // TODO add menu to quit
    auto app = Gtk::Application::create(argc, argv, "com.tinybitofgiantsblood.tbgb");

    Framebuf fb;
    Animations anim(fb);
    Viz viz(fb);
    MainWindow window(viz, anim.get_all());

    return app->run(window);
}
