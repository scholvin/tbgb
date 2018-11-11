#include "animations.h"
#include <functional>
#include <iostream>

Animations::Animations(Framebuf& fb) : m_fb(fb)
{
    // don't love this pattern, but we gotta move along
    m_list.push_back(std::make_pair("blackout", std::bind(&Animations::blackout, this)));
    m_list.push_back(std::make_pair("whiteout", std::bind(&Animations::whiteout, this)));
}

Animations::~Animations()
{
}

void
Animations::blackout(void)
{
    std::cout << "animation: blackout" << std::endl;
    std::lock_guard<std::mutex> lock(m_fb.mutex());
    for (int x = 0; x < TBGB_XMAX; x++)
        for (int y = 0; y < TBGB_YMAX; y++)
            m_fb.data(x, y).red = m_fb.data(x, y).green = m_fb.data(x, y).blue = 0;
}

void
Animations::whiteout(void)
{
    std::cout << "animation: whiteout" << std::endl;
    std::lock_guard<std::mutex> lock(m_fb.mutex());
    for (int x = 0; x < TBGB_XMAX; x++)
        for (int y = 0; y < TBGB_YMAX; y++)
            m_fb.data(x, y).red = m_fb.data(x, y).green = m_fb.data(x, y).blue = 1;
}
