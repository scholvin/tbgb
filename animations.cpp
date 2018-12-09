#include "animations.h"
#include <functional>
#include <iostream>
#include <thread>

namespace {
    // delays are all in milliseconds
    const int FLASH_DELAY = 250;
    const int FOO_TO_FULL_STEPS = 100;
    const int FOO_TO_FULL_DELAY = 10;
    const int TOP_DOWN_WAVE_DELAY = 50;
    const int LEFT_RIGHT_WAVE_DELAY = 35;
    const int TBGB_DELAY = 250;
}

Animations::Animations(Framebuf& fb, RenderFuncType r1, RenderFuncType r2) : m_fb(fb), m_r1(r1), m_r2(r2),
    m_canceling(false),
    m_color(Framebuf::BLACK),
    m_master(1),
    m_rainbow_lastc(0)

{
    // don't love this pattern, but we gotta move along
    m_list.push_back(std::make_tuple("blackout", std::bind(&Animations::blackout, this), nullptr));
    m_list.push_back(std::make_tuple("whiteout", std::bind(&Animations::whiteout, this), &Framebuf::INCANDESCENT));
    m_list.push_back(std::make_tuple("flash", std::bind(&Animations::flash, this), &Framebuf::INCANDESCENT));
    m_list.push_back(std::make_tuple("0 to 100", std::bind(&Animations::foo_to_full, this, 0.0), &Framebuf::INCANDESCENT));
    m_list.push_back(std::make_tuple("50 to 100", std::bind(&Animations::foo_to_full, this, 0.5), &Framebuf::INCANDESCENT));
    m_list.push_back(std::make_tuple("top down", std::bind(&Animations::top_down_wave, this), &Framebuf::INCANDESCENT));
    m_list.push_back(std::make_tuple("left right", std::bind(&Animations::left_right_wave, this), &Framebuf::INCANDESCENT));
    m_list.push_back(std::make_tuple("TBGB all", std::bind(&Animations::TBGB, this, true), &Framebuf::INCANDESCENT));
    m_list.push_back(std::make_tuple("TBGB each", std::bind(&Animations::TBGB, this, false), &Framebuf::INCANDESCENT));


#if 0
    m_list.push_back(std::make_pair("rainbow", std::bind(&Animations::rainbow, this)));

    // this may or may not ever go away
    m_list.push_back(std::make_pair("colorwheel", std::bind(&Animations::colorwheel, this)));

    // this will go away, after proving out the framebuf, etc
    m_list.push_back(std::make_pair("linetest", std::bind(&Animations::linetest, this)));
    // these will go away after proving DMX layout, etc
    m_list.push_back(std::make_pair("T1", std::bind(&Animations::one_letter_test, this, T1_START)));
    m_list.push_back(std::make_pair("B2", std::bind(&Animations::one_letter_test, this, B2_START)));
    m_list.push_back(std::make_pair("G3", std::bind(&Animations::one_letter_test, this, G3_START)));
    m_list.push_back(std::make_pair("B4", std::bind(&Animations::one_letter_test, this, B4_START)));

    m_list.push_back(std::make_pair("demo", std::bind(&Animations::demo, this)));
#endif
}

Animations::~Animations()
{
}

void
Animations::set_global_colors(double red, double green, double blue)
{
    std::lock_guard<std::mutex> lock(m_colorMutex);
    m_color.set_red(red);
    m_color.set_green(green);
    m_color.set_blue(blue);
}

Framebuf::Color
Animations::get_global_color()
{
    Framebuf::Color ret;
    double mult;
    {
        std::lock_guard<std::mutex> lock(m_colorMutex);
        ret = m_color;
    }
    {
        std::lock_guard<std::mutex> lock(m_masterMutex);
        mult = m_master;
    }
    dim(ret, mult);
    return ret;
}

void
Animations::set_master(double master)
{
    std::lock_guard<std::mutex> lock(m_masterMutex);
    m_master = master;
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
        {
            for (int y = 0; y < TBGB_YMAX; y++)
            {
                // this is hardcoded, don't consult the global color
                m_fb.data(x, y) = Framebuf::BLACK;
            }
        }
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
        {
            for (int y = 0; y < TBGB_YMAX; y++)
            {
                m_fb.data(x, y) = get_global_color();
            }
        }
    }
    RENDER;
    return true;
}

bool
Animations::flash(void)
{
    if (!whiteout()) return false;
    SLEEPMS(FLASH_DELAY);
    if (!blackout()) return false;
    SLEEPMS(FLASH_DELAY);
    return true;
}

bool
Animations::foo_to_full(double start)
{
    for (double d = start; d <= 1.0; d += (1.0 - start) / FOO_TO_FULL_STEPS)
    {
        Framebuf::Color color = get_global_color();
        dim(color, d);
        {
            LOCK;
            for (int x = 0; x < TBGB_XMAX; x++)
            {
                for (int y = 0; y < TBGB_YMAX; y++)
                {
                    m_fb.data(x, y) = color;
                }
            }
        }
        RENDER;
        SLEEPMS(FOO_TO_FULL_DELAY);
    }
    return true;
}

bool
Animations::top_down_wave(void)
{
    if (!blackout()) return false;
    SLEEPMS(TOP_DOWN_WAVE_DELAY);
    for (int y = 0; y < TBGB_YMAX; y++)
    {
        {
            LOCK;
            for (int x = 0; x < TBGB_XMAX; x++)
            {
                m_fb.data(x, y) = get_global_color();
            }
        }
        RENDER;
        SLEEPMS(TOP_DOWN_WAVE_DELAY);
    }
    return true;
}

bool
Animations::left_right_wave(void)
{
    if (!blackout()) return false;
    SLEEPMS(LEFT_RIGHT_WAVE_DELAY);
    for (int x = 0; x < TBGB_XMAX; x++)
    {
        {
            LOCK;
            for (int y = 0; y < TBGB_YMAX; y++)
            {
                m_fb.data(x, y) = get_global_color();
            }
        }
        RENDER;
        SLEEPMS(LEFT_RIGHT_WAVE_DELAY);
    }
    return true;
}

bool
Animations::TBGB(bool cumulative)
{
    if (!blackout()) return false;
    const int lmax[] = { T1_START+LETTER_WIDTH, B2_START+LETTER_WIDTH, G3_START+LETTER_WIDTH, B4_START+LETTER_WIDTH };
    int x = 0;
    for (int l = 0; l < 4; l++)
    {
        {
            if (!cumulative)
            {
                if (!blackout()) return false;
            }
            LOCK;
            for (; x < lmax[l]; x++)
            {
                for (int y = 0; y < TBGB_YMAX; y++)
                {
                    m_fb.data(x, y) = get_global_color();
                }
            }
        }
        RENDER;
        SLEEPMS(TBGB_DELAY);
    }
    if (cumulative)
    {
        if (!blackout()) return false;
        SLEEPMS(TBGB_DELAY);
    }
#if 0
    // this code is a shout-out to the old G-O-O-D lights
    for (int i = 0; i < 4; i++)
    {
        if (!blackout()) return false;
        SLEEPMS(DELAY);
        if (!whiteout()) return false;
        SLEEPMS(DELAY);
    }
    if (!blackout()) return false;
    SLEEPMS(DELAY);
#endif
    return true;
}
        
bool
Animations::linetest(void)
{
    {
        LOCK;
        m_fb.line(0, 0, TBGB_XMAX-1, TBGB_YMAX-1, Framebuf::ORANGE);
        m_fb.line(TBGB_XMAX-1, 0, 0, TBGB_YMAX-1, Framebuf::GREEN);
    }
    RENDER;
    return true;
}

bool
Animations::rainbow(void)
{
    Framebuf::Color colors[] = { Framebuf::RED, Framebuf::ORANGE, Framebuf::YELLOW, Framebuf::GREEN,
                                 Framebuf::BLUE, Framebuf::PURPLE, Framebuf::BLACK };
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
#if 0
bool
Animations::colorwheel(void)
{
    double red, green, blue;
    {
        std::scoped_lock<std::mutex> lock(m_colorMutex);
        red = m_globalRed;
        green = m_globalGreen;
        blue = m_globalBlue;
    }
    {
        LOCK;
        for (int x = 0; x < TBGB_XMAX; x++)
            for (int y = 0; y < TBGB_YMAX; y++)
            {
                m_fb.data(x, y).set_red(red);
                m_fb.data(x, y).set_green(green);
                m_fb.data(x, y).set_blue(blue);
            }
    }
    RENDER;
    return true;
}

bool
Animations::demo(void)
{
    double red, green, blue;
    {
        std::scoped_lock<std::mutex> lock(m_colorMutex);
        red = m_globalRed;
        green = m_globalGreen;
        blue = m_globalBlue;
    }

    const unsigned MILLIS_PER_BEAT = 420;
    const unsigned OFF_TIME = 50;

    const int lmax[] = { T1_START+LETTER_WIDTH, B2_START+LETTER_WIDTH, G3_START+LETTER_WIDTH, B4_START+LETTER_WIDTH };
    int x = 0;
    for (int l = 0; l < 4; l++)
    {
        {
            LOCK;
            for (; x < lmax[l]; x++)
            {
                for (int y = 0; y < TBGB_YMAX; y++)
                {
                    m_fb.data(x, y).set_red(red);
                    m_fb.data(x, y).set_green(green);
                    m_fb.data(x, y).set_blue(blue);
                }
            }
        }
        RENDER;
        SLEEPMS(MILLIS_PER_BEAT - OFF_TIME);
        blackout();
        SLEEPMS(OFF_TIME);
    }
    for (int i = 0; i < 2; i++)
    {
        colorwheel();
        SLEEPMS(MILLIS_PER_BEAT - OFF_TIME);
        blackout();
        SLEEPMS(OFF_TIME);
    } 
    SLEEPMS(1000);
    return true;
}

#endif
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
                m_fb.data(x0, y0) = Framebuf::BLACK;
                m_fb.data(x, y) = Framebuf::WHITE;
            }
            RENDER;
            x0 = x;
            y0 = y;
            SLEEPMS(250);
        }
    }
    return true;
}

void
Animations::dim(Framebuf::Color& color, double mult)
{
    if (mult < 0 || mult > 1)
        return;
    color.set_red(mult * color.get_red());
    color.set_green(mult * color.get_green());
    color.set_blue(mult * color.get_blue());
}

