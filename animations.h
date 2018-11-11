#pragma once

#include <vector>
#include <string>
#include <utility>
#include "framebuf.h"

class Animations
{
public:
    typedef std::function<void(void)> FuncType;
    typedef std::pair<std::string, FuncType> Identifier;
    typedef std::vector<Identifier> AnimationList;;

    Animations(Framebuf& fb);

    ~Animations();

    const AnimationList& get_all() const { return m_list; }

    // the animations
    void blackout();
    void whiteout();

private:
    Framebuf& m_fb;
    AnimationList m_list;
};

