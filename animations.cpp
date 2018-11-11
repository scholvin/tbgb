#include "animations.h"
#include <functional>
#include <iostream>

Animations::Animations(Framebuf& fb, RenderFuncType r1, RenderFuncType r2) : m_fb(fb), m_r1(r1), m_r2(r2)
{
    // don't love this pattern, but we gotta move along
    m_list.push_back(std::make_pair("blackout", std::bind(&Animations::blackout, this)));
    m_list.push_back(std::make_pair("whiteout", std::bind(&Animations::whiteout, this)));
}

Animations::~Animations()
{
}

void
Animations::render()
{
    if (m_r1) m_r1();
    if (m_r2) m_r2();
}

void
Animations::blackout(void)
{
    std::cout << "animation: blackout" << std::endl;
    {
        std::lock_guard<std::mutex> lock(m_fb.mutex());
        for (int x = 0; x < TBGB_XMAX; x++)
            for (int y = 0; y < TBGB_YMAX; y++)
                m_fb.data(x, y).red = m_fb.data(x, y).green = m_fb.data(x, y).blue = 0;
    }
    render();
}

void
Animations::whiteout(void)
{
    std::cout << "animation: whiteout" << std::endl;
    {
        std::lock_guard<std::mutex> lock(m_fb.mutex());
        for (int x = 0; x < TBGB_XMAX; x++)
            for (int y = 0; y < TBGB_YMAX; y++)
                m_fb.data(x, y).red = m_fb.data(x, y).green = m_fb.data(x, y).blue = 1;
    }
    render();
}
