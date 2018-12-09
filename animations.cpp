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
    const int EDGE_CHASE_DELAY = 4;
    const int EDGE_TRAIN_LENGTH = 3;
    const int ONE_BY_ONE_DELAY = 3;
    const int INSIDE_OUT_DELAY = 150;

    const std::vector<Animations::_pt> T1_PERIMETER = {
        {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0},
        {8, 1}, {8, 2}, {7, 2}, {6, 2}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
        {5, 7}, {5, 8}, {5, 9}, {5, 10}, {5, 11}, {5, 12}, {5, 13}, {5, 14}, {5, 15},
        {5, 16}, {4, 16}, {3, 16}, {3, 15}, {3, 14}, {3, 13}, {3, 12}, {3, 11}, {3, 10},
        {3, 9}, {3, 8}, {3, 7}, {3, 6}, {3, 5}, {3, 4}, {3, 3}, {3, 2}, {2, 2}, {1, 2},
        {0, 2}, {0, 1}
    };

    const std::vector<Animations::_pt> B2_PERIMETER = {
        {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}, {17, 0}, {18, 1},
        {18, 2}, {18, 3}, {18, 4}, {18, 5}, {18, 6}, {18, 6}, {18, 7}, {17, 8}, {18, 9},
        {18, 10}, {18, 11}, {18, 12}, {18, 13}, {18, 14}, {18, 15}, {17, 16}, {16, 16},
        {15, 16}, {15, 16}, {14, 16}, {13, 16}, {12, 16}, {11, 16}, {10, 16}, {10, 15},
        {10, 14}, {10, 13}, {10, 12}, {10, 11}, {10, 10}, {10, 9}, {10, 8}, {10, 7},
        {10, 6}, {10, 5}, {10, 4}, {10, 3}, {10, 2}, {10, 1}
    };

    const std::vector<Animations::_pt> G3_PERIMETER = {
        {21, 0}, {22, 0}, {23, 0}, {24, 0}, {25, 0}, {26, 0}, {27, 0}, {28, 0}, {28, 1},
        {28, 2}, {27, 2}, {26, 2}, {25, 2}, {24, 5}, {23, 2}, {22, 2}, {22, 3}, {22, 4},
        {22, 5}, {22, 6}, {22, 7}, {22, 8}, {22, 9}, {22, 10}, {22, 11}, {22, 12}, {22, 13},
        {22, 14}, {23, 14}, {24, 14}, {25, 14}, {26, 14}, {26, 13}, {26, 12}, {26, 11}, {26, 10},
        {26, 9}, {26, 8}, {26, 7}, {27, 7}, {28, 7}, {28, 8}, {28, 9}, {28, 10}, {28, 11},
        {28, 12}, {28, 13}, {28, 14}, {28, 15}, {28, 16}, {27, 16}, {26, 16}, {25, 16}, {24, 16},
        {23, 16}, {22, 16}, {21, 16}, {20, 15}, {20, 14}, {20, 13}, {20, 12}, {20, 11}, {20, 10},
        {20, 9}, {20, 8}, {20, 7}, {20, 6}, {20, 5}, {20, 4}, {20, 3}, {20, 2}, {20, 1}
    };

    const std::vector<Animations::_pt> B4_PERIMETER = {
        {30, 0}, {31, 0}, {32, 0}, {33, 0}, {34, 0}, {35, 0}, {36, 0}, {37, 0}, {38, 1},
        {38, 2}, {38, 3}, {38, 4}, {38, 5}, {38, 6}, {38, 6}, {38, 7}, {37, 8}, {38, 9},
        {38, 10}, {38, 11}, {38, 12}, {38, 13}, {38, 14}, {38, 15}, {37, 16}, {36, 16},
        {35, 16}, {35, 16}, {34, 16}, {33, 16}, {32, 16}, {31, 16}, {30, 16}, {30, 15},
        {30, 14}, {30, 13}, {30, 12}, {30, 11}, {30, 10}, {30, 9}, {30, 8}, {30, 7},
        {30, 6}, {30, 5}, {30, 4}, {30, 3}, {30, 2}, {30, 1}
    };

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
    m_list.push_back(std::make_tuple("edge chase", std::bind(&Animations::edge_chase, this), &Framebuf::INCANDESCENT));
    m_list.push_back(std::make_tuple("one by one", std::bind(&Animations::one_by_one, this), &Framebuf::INCANDESCENT));
    m_list.push_back(std::make_tuple("inside out", std::bind(&Animations::inside_out, this), &Framebuf::INCANDESCENT));
    m_list.push_back(std::make_tuple("outside in", std::bind(&Animations::outside_in, this), &Framebuf::INCANDESCENT));

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
    // probably some cute way to do this in the static namespace above but no time
    m_letter_perimeters = T1_PERIMETER;
    m_letter_perimeters.insert(m_letter_perimeters.end(), B2_PERIMETER.begin(), B2_PERIMETER.end());
    m_letter_perimeters.insert(m_letter_perimeters.end(), G3_PERIMETER.begin(), G3_PERIMETER.end());
    m_letter_perimeters.insert(m_letter_perimeters.end(), B4_PERIMETER.begin(), B4_PERIMETER.end());
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

void
Animations::dim(Framebuf::Color& color, double mult)
{
    if (mult < 0 || mult > 1)
        return;
    color.set_red(mult * color.get_red());
    color.set_green(mult * color.get_green());
    color.set_blue(mult * color.get_blue());
}

// ------------------------------ animations from this point on  ----------------------------

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
Animations::edge_chase()
{
    if (!blackout()) return false;
    m_et.clear();
    for (auto it: m_letter_perimeters)
    {
        m_et.push_front(it);
        if (m_et.size() > EDGE_TRAIN_LENGTH)
        {
            LOCK;
            m_fb.data(m_et.back().x, m_et.back().y) = Framebuf::BLACK;
            m_et.pop_back();
        }
        for (auto train: m_et)
        {
            LOCK;
            m_fb.data(train.x, train.y) = get_global_color();
        }
        RENDER;
        SLEEPMS(EDGE_CHASE_DELAY);
    }
    return true;
}

bool
Animations::one_by_one(void)
{
    int x0 = -1;
    int y0 = -1;
    if (!blackout()) return false;
    for (int y = 0; y < TBGB_YMAX; y++)
    {
        for (int x = 0; x < TBGB_XMAX; x++)
        {
             if (!is_visible(x, y))
                continue;
            {
                LOCK;
                m_fb.data(x0, y0) = Framebuf::BLACK;
                m_fb.data(x, y) = get_global_color();
                x0 = x;
                y0 = y;
            }
            RENDER;
            SLEEPMS(ONE_BY_ONE_DELAY);
        }
    }
    return true;
}

bool
Animations::inside_out(void)
{
    int x = 8;
    int y = 8;
    int w = 22;
    int h = 0;

    if (!blackout()) return false;

    bool done = false;
    while (!done)
    {
        {
            LOCK;
            if (h == 0)
            {
                m_fb.line(x, y, x + w, y, get_global_color());
            }
            else
            {
                m_fb.line(x, y, x + w, y, get_global_color());
                m_fb.line(x + w, y, x + w, y + h, get_global_color());
                m_fb.line(x + w, y + h, x, y + h, get_global_color());
                m_fb.line(x, y + h, x, y, get_global_color());
            }
        }
        RENDER;
        SLEEPMS(INSIDE_OUT_DELAY);
        x--;
        y--;
        w += 2;
        h += 2;
        if (x + w >= TBGB_XMAX)
            done = true;
    }
    if (!blackout()) return false;
    SLEEPMS(INSIDE_OUT_DELAY);
    return true;
}

bool
Animations::outside_in()
{
    int x = 0;
    int y = 0;
    int w = TBGB_XMAX - 1;
    int h = TBGB_YMAX - 1;

    if (!blackout()) return false;

    bool done = false;
    while (!done)
    {
        {
            LOCK;
            if (h <= 0)
            {
                m_fb.line(x, y, x + w, y, get_global_color());
                done = true;
                std::cout << "x, y, w, h " << x << " " << y << " " << w << " " << h << std::endl;
            }
            else
            {
                m_fb.line(x, y, x + w, y, get_global_color());
                m_fb.line(x + w, y, x + w, y + h, get_global_color());
                m_fb.line(x + w, y + h, x, y + h, get_global_color());
                m_fb.line(x, y + h, x, y, get_global_color());
            }
        }
        RENDER;
        SLEEPMS(INSIDE_OUT_DELAY);
        x++;
        y++;
        w -= 2;
        h -= 2;
    }
    if (!blackout()) return false;
    SLEEPMS(INSIDE_OUT_DELAY);
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
Animations::one_letter_test(int start)
{
    if (!blackout()) return false;
    int x0 = -1;
    int y0 = -1;
    for (int y = 0; y < TBGB_YMAX; y++)
    {
        for (int x = start; x < start+LETTER_WIDTH; x++)
        {
            if (!is_visible(x, y))
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
#endif
