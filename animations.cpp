#include "animations.h"
#include <functional>
#include <iostream>
#include <thread>

Animations::Animations(Framebuf& fb, RenderFuncType r1, RenderFuncType r2) : m_fb(fb), m_r1(r1), m_r2(r2)
{
    // don't love this pattern, but we gotta move along
    m_list.push_back(std::make_pair("blackout", std::bind(&Animations::blackout, this)));
    m_list.push_back(std::make_pair("whiteout", std::bind(&Animations::whiteout, this)));
    m_list.push_back(std::make_pair("TBGB", std::bind(&Animations::TBGB, this)));
    m_list.push_back(std::make_pair("linetest", std::bind(&Animations::linetest, this)));
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

#define LOCK std::lock_guard<std::mutex> lock(m_fb.mutex())
#define SLEEPMS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

void
Animations::blackout(void)
{
    {
        LOCK;
        for (int x = 0; x < TBGB_XMAX; x++)
            for (int y = 0; y < TBGB_YMAX; y++)
                m_fb.data(x, y).red = m_fb.data(x, y).green = m_fb.data(x, y).blue = 0;
    }
    render();
}

void
Animations::whiteout(void)
{
    {
        LOCK;
        for (int x = 0; x < TBGB_XMAX; x++)
            for (int y = 0; y < TBGB_YMAX; y++)
                m_fb.data(x, y).red = m_fb.data(x, y).green = m_fb.data(x, y).blue = 1;
    }
    render();
}

void
Animations::TBGB(void)
{
    blackout();

    const int DELAY = 250;

    const int lmax[] = {9, 19, 29, 39};
    int x = 0;
    for (int l = 0; l < 4; l++)
    {
        {
            LOCK;
            for (; x < lmax[l]; x++)
            {
                for (int y = 0; y < TBGB_YMAX; y++)
                    m_fb.data(x, y).red = m_fb.data(x, y).green = m_fb.data(x, y).blue = 1;
            }
        }
        render();
        SLEEPMS(DELAY);
    }
    for (int i = 0; i < 4; i++)
    {
        blackout();
        SLEEPMS(DELAY);
        whiteout();
        SLEEPMS(DELAY);
    }
    blackout();
    SLEEPMS(DELAY);
}
        
void
Animations::linetest(void)
{
    Framebuf::Data orange = { 1, 0.65, 0 };
    Framebuf::Data green = { 0, 1, 0 };
    {
        LOCK;
        m_fb.line(0, 0, TBGB_XMAX-1, TBGB_YMAX-1, orange);
        m_fb.line(TBGB_XMAX-1, 0, 0, TBGB_YMAX-1, green);
    }
    render();
}


