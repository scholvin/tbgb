#pragma once

#include <vector>
#include <string>
#include <utility>
#include "framebuf.h"

// home to the animation code
class Animations
{
public:
    typedef std::function<bool(void)> AnimFuncType;
    typedef std::tuple<std::string, AnimFuncType, const Framebuf::Color*> Identifier;
    typedef std::vector<Identifier> AnimationList;;

    typedef std::function<void(void)> RenderFuncType;

    Animations(Framebuf& fb, RenderFuncType r1, RenderFuncType r2);

    ~Animations();

    const AnimationList& get_all() const { return m_list; }

    // from the color window scroller
    void set_global_colors(double red, double green, double blue);
    void set_master(double master);

    // stop the current animation - bail out on next render
    void cancel();

    // the animations - I suppose these could be private since the binding is here
    // they return true if they completed, false if interrupted
    bool blackout();
    bool whiteout();
    bool TBGB();
    bool linetest();
    bool rainbow();
    bool colorwheel();
    bool demo();

private:
    Framebuf& m_fb;
    AnimationList m_list;
    RenderFuncType m_r1, m_r2;

    bool one_letter_test(int start);

    // return true if it's OK to keep going, false if time to stop
    bool render();

    // apply the master times the color
    Framebuf::Color get_global_color();

    // is an animation canceling?
    std::atomic<bool> m_canceling;

    std::mutex m_colorMutex;
    Gdk::RGBA m_color;

    std::mutex m_masterMutex;
    double m_master;

    // animation specific stuff
    int m_rainbow_lastc;
};

