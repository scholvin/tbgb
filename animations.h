#pragma once

#include <vector>
#include <string>
#include <utility>
#include "framebuf.h"

// home to the animation code
class Animations
{
public:
    typedef std::function<void(void)> AnimFuncType;
    typedef std::pair<std::string, AnimFuncType> Identifier;
    typedef std::vector<Identifier> AnimationList;;

    typedef std::function<void(void)> RenderFuncType;

    Animations(Framebuf& fb, RenderFuncType r1, RenderFuncType r2);

    ~Animations();

    const AnimationList& get_all() const { return m_list; }

    // the animations - I suppose these could be private since the binding is here
    void blackout();
    void whiteout();
    void TBGB();
    void linetest();
    void rainbow();

private:
    Framebuf& m_fb;
    AnimationList m_list;
    RenderFuncType m_r1, m_r2;

    void one_letter_test(int start);

    void render();

    // animation specific stuff
    int m_rainbow_lastc;
};

