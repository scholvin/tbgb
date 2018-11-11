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

    // the animations
    void blackout();
    void whiteout();
    void TBGB();

private:
    Framebuf& m_fb;
    AnimationList m_list;
    RenderFuncType m_r1, m_r2;

    void render();
};

