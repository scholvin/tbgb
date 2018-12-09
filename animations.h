#pragma once

#include <vector>
#include <string>
#include <utility>
#include <random>
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
    bool flash();
    bool foo_to_full(double start);
    bool top_down_wave();
    bool left_right_wave();
    bool TBGB(bool cumulative);
    bool edge_chase();
    bool one_by_one();
    bool inside_out();
    bool outside_in();
    bool twinkle();

    bool rainbow();


    // for points...
    struct _pt { int x, y; };

private:
    Framebuf& m_fb;
    AnimationList m_list;
    RenderFuncType m_r1, m_r2;
  
    // is an animation canceling?
    std::atomic<bool> m_canceling;

    std::mutex m_colorMutex;
    Gdk::RGBA m_color;

    std::mutex m_masterMutex;
    double m_master;

    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_int_distribution<> m_xdist;
    std::uniform_int_distribution<> m_ydist;

    // animation specific stuff
    int m_rainbow_lastc;

    // edge train
    std::deque<_pt> m_et;
    std::vector<_pt> m_letter_perimeters;

    // twinkle twinkle
    std::deque<_pt> m_stars;

    // return true if it's OK to keep going, false if time to stop
    bool render();

    // apply the master times the color
    Framebuf::Color get_global_color();

    // "dim" the color specified by multiplying by mult (between 0 and 1)
    static void dim(Framebuf::Color& color, double mult);

    // return a random, visible point
    _pt random_pt();

    // inclusive
    int random_num(int min, int max);


#if 0
    // these guys are just for testing
    bool one_letter_test(int start);
    bool linetest();
    bool colorwheel();
    bool demo();
#endif    
  
};

