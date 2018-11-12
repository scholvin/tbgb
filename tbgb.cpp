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

class MainWindow : public Gtk::Window
{
public:
    MainWindow(Viz& viz, const Animations::AnimationList& animations) : m_viz(viz), m_current(nullptr), m_running(true), m_done(false)
    {
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
                m_buttons[i].signal_clicked().connect(
                    sigc::bind<std::string, Animations::AnimFuncType>(sigc::mem_fun(this, &MainWindow::change_animation), 
                                                                      animations[i].first, animations[i].second));
            }
            else
            {
                m_buttons[i].set_label("animation " + std::to_string(i+1));
                m_buttons[i].signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(this, &MainWindow::not_impl), i+1));
            }
            m_grid.attach(m_buttons[i], i % 10, i / 10, 1, 1);
        }

        show_all();
        m_thread = std::thread(std::bind(&MainWindow::animation_runner, this));
        m_thread.detach();
    }

    void change_animation(const std::string& name, Animations::AnimFuncType animation)
    {
        std::cout << "change_animation to " << name << std::endl;
        m_current = animation;
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

    // current animation
    Animations::AnimFuncType m_current;

    // parallelism
    std::thread m_thread;
    std::atomic_bool m_running;
    std::atomic_bool m_done;

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
    DMX dmx(fb);
    Animations anim(fb, std::bind(&Viz::render, &viz), std::bind(&DMX::render, &dmx));
    MainWindow window(viz, anim.get_all());

    auto ret = app->run(window);
    std::cout << "exiting with code " << ret << std::endl;
    return ret;
}
