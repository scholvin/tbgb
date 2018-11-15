#include "animations.h"
#include <functional>
#include <iostream>
#include <thread>

Animations::Animations(Framebuf& fb, RenderFuncType r1, RenderFuncType r2) : m_fb(fb), m_r1(r1), m_r2(r2),
    m_canceling(false),
    m_rainbow_lastc(0)

{
    // don't love this pattern, but we gotta move along
    m_list.push_back(std::make_pair("blackout", std::bind(&Animations::blackout, this)));
    m_list.push_back(std::make_pair("whiteout", std::bind(&Animations::whiteout, this)));
    m_list.push_back(std::make_pair("TBGB", std::bind(&Animations::TBGB, this)));
    m_list.push_back(std::make_pair("rainbow", std::bind(&Animations::rainbow, this)));

    // this will go away, after proving out the framebuf, etc
    m_list.push_back(std::make_pair("linetest", std::bind(&Animations::linetest, this)));
    // these will go away after proving DMX layout, etc
    m_list.push_back(std::make_pair("T1", std::bind(&Animations::one_letter_test, this, T1_START)));
    m_list.push_back(std::make_pair("B2", std::bind(&Animations::one_letter_test, this, B2_START)));
    m_list.push_back(std::make_pair("G3", std::bind(&Animations::one_letter_test, this, G3_START)));
    m_list.push_back(std::make_pair("B4", std::bind(&Animations::one_letter_test, this, B4_START)));
}

Animations::~Animations()
{
}

bool
Animations::render()
{
    if (m_canceling)
    {
        m_canceling = false;
        return false;
    }
    if (m_r1) m_r1();
    if (m_r2) m_r2();
    return true;
}

void
Animations::cancel()
{
    m_canceling = true;
}

#define LOCK std::lock_guard<std::mutex> lock(m_fb.mutex())
#define SLEEPMS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
#define RENDER { if (!render()) return false; }

bool
Animations::blackout(void)
{
    {
        LOCK;
        for (int x = 0; x < TBGB_XMAX; x++)
            for (int y = 0; y < TBGB_YMAX; y++)
                m_fb.data(x, y).red = m_fb.data(x, y).green = m_fb.data(x, y).blue = 0;
    }
    RENDER;
    return true;
}

bool
Animations::whiteout(void)
{
    {
        LOCK;
        for (int x = 0; x < TBGB_XMAX; x++)
            for (int y = 0; y < TBGB_YMAX; y++)
                m_fb.data(x, y).red = m_fb.data(x, y).green = m_fb.data(x, y).blue = 1;
    }
    RENDER;
    return true;
}

bool
Animations::TBGB(void)
{
    blackout();

    const int DELAY = 250;

    const int lmax[] = { T1_START+LETTER_WIDTH, B2_START+LETTER_WIDTH, G3_START+LETTER_WIDTH, B4_START+LETTER_WIDTH };
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
        RENDER;
        SLEEPMS(DELAY);
    }
    for (int i = 0; i < 4; i++)
    {
        if (!blackout()) return false;
        SLEEPMS(DELAY);
        if (!whiteout()) return false;
        SLEEPMS(DELAY);
    }
    if (!blackout()) return false;
    SLEEPMS(DELAY);
    return true;
}
        
bool
Animations::linetest(void)
{
    Framebuf::Data orange = { 1, 0.65, 0 };
    Framebuf::Data green = { 0, 1, 0 };
    {
        LOCK;
        m_fb.line(0, 0, TBGB_XMAX-1, TBGB_YMAX-1, orange);
        m_fb.line(TBGB_XMAX-1, 0, 0, TBGB_YMAX-1, green);
    }
    RENDER;
    return true;
}

bool
Animations::rainbow(void)
{
    // TODO make colors sane
    Framebuf::Data red = { 1, 0, 0};
    Framebuf::Data orange = { 1, 0.65, 0 };
    Framebuf::Data yellow = { 1, 1, 0 };
    Framebuf::Data green = { 0, 1, 0 };
    Framebuf::Data blue = { 0, 0, 1 };
    Framebuf::Data purple = {138/255.0 ,43/255.0 ,226/255.0};
    Framebuf::Data black = { 0, 0, 0};

    Framebuf::Data colors[] = { red, orange, yellow, green, blue, purple, black };
    int CMAX = 6;
    m_rainbow_lastc = (m_rainbow_lastc + 1 ) % CMAX;
    int c = m_rainbow_lastc;

    for (int x = 0; x < TBGB_XMAX + TBGB_YMAX - 2; x++)
    {
        {
            LOCK;
            m_fb.line(x, 0, 0, x, colors[c]);
        }
        RENDER;
        SLEEPMS(20);
        c = (c + 1) % CMAX;
    }
    return true;
}

bool
Animations::one_letter_test(int start)
{
    if (!blackout()) return false;
    int x0 = -1;
    int y0 = -1;
    for (int y = 0; y < TBGB_YMAX; y++)
    {
        for (int x = start; x < start+LETTER_WIDTH; x++)
        {
            if (!isVisible(x, y))
                continue;
            {
                LOCK;
                m_fb.data(x0, y0).red = m_fb.data(x0, y0).green = m_fb.data(x0, y0).blue = 0;
                m_fb.data(x, y).red = m_fb.data(x, y).green = m_fb.data(x, y).blue = 1;
            }
            RENDER;
            x0 = x;
            y0 = y;
            SLEEPMS(250);
        }
    }
    return true;
}
