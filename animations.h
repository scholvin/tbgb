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
    typedef std::pair<std::string, AnimFuncType> Identifier;
    typedef std::vector<Identifier> AnimationList;;

    typedef std::function<void(void)> RenderFuncType;

    Animations(Framebuf& fb, RenderFuncType r1, RenderFuncType r2);

    ~Animations();

    const AnimationList& get_all() const { return m_list; }

    // stop the current animation - bail out on next render
    void cancel();

    // the animations - I suppose these could be private since the binding is here
    // they return true if they completed, false if interrupted
    bool blackout();
    bool whiteout();
    bool TBGB();
    bool linetest();
    bool rainbow();

private:
    Framebuf& m_fb;
    AnimationList m_list;
    RenderFuncType m_r1, m_r2;

    bool one_letter_test(int start);

    // return true if it's OK to keep going, false if time to stop
    bool render();

    std::atomic<bool> m_canceling;

    // animation specific stuff
    int m_rainbow_lastc;
};

