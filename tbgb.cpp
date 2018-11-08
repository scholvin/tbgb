#include <gtkmm.h>
#include <array>
#include <vector>
#include <iostream>
#include "viz.h"
#include "framebuf.h"
#include "global.h"

// TODO move me
class Animations
{
public:
    static void blackout(int x) { std::cout << "blackout" << std::endl; }
    static void whiteout(int x) { std::cout << "whiteout" << std::endl; }
    static void not_impl(int x) { std::cout << "animation " << x << " not implemented yet" << std::endl; }
};

class MainWindow : public Gtk::Window
{
public:
    MainWindow(Viz& viz) : m_viz(viz)
    {
        //set_default_size(1430, 650);
        set_title("TBGB");
        add(m_vbox);
        m_vbox.set_homogeneous(false);
        m_vbox.pack_start(m_viz, true, true);
        m_vbox.pack_end(m_grid, false, false);
        m_grid.set_hexpand(true);

        // TODO move this out of this class, pass vector in as ref
        typedef void (*AnimationFunc)(int);
        std::vector<std::pair<std::string, AnimationFunc>> animations = 
        {
            std::make_pair("blackout", &Animations::blackout),
            std::make_pair("whiteout", &Animations::whiteout)
        };

        for (auto i = 0; i < 20; ++i)
        {
            // TODO bigger font on label text
            m_buttons[i].set_hexpand(true);
            if (i < animations.size())
            {
                m_buttons[i].set_label(animations[i].first);
                m_buttons[i].signal_clicked().connect(sigc::bind<int>(sigc::ptr_fun(animations[i].second), 0));
            }
            else
            {
                m_buttons[i].set_label("animation " + std::to_string(i+1));
                m_buttons[i].signal_clicked().connect(sigc::bind<int>(sigc::ptr_fun(&Animations::not_impl), i+1));
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
    Viz viz(fb);
    MainWindow window(viz);

    return app->run(window);
}
