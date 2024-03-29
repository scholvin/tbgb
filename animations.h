#pragma once

#include <vector>
#include <string>
#include <utility>
#include <random>
#include <atomic>
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
    bool whiteout(double percent);
    bool flash();
    bool foo_to_full(double start, bool oscillate);
    bool top_down_wave();
    bool left_right_wave();
    bool TBGB(bool cumulative);
    bool edge_chase();
    bool one_by_one();
    bool inside_out();
    bool outside_in();
    bool twinkle(bool color);
    bool pregame();
    bool impulse();

    bool rainbow();
    bool rotate3(const Framebuf::Color* one, const Framebuf::Color* two, const Framebuf::Color* three);

    // for points...
    struct _pt 
    { 
        int x, y; 
        bool operator==(const _pt& other) const { return x == other.x && y == other.y; };
    };

private:
    Framebuf& m_fb;
    AnimationList m_list;
    RenderFuncType m_r1, m_r2;
  
    // is an animation canceling?
    std::atomic<bool> m_canceling;

    // is an animation newly changed (to do something different on first render)
    std::atomic<bool> m_new;

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
    std::deque<Framebuf::Color> m_star_colors;
    bool m_last_twinkle_color;

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

