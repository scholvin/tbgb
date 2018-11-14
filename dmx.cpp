#include <ola/Logging.h>
#include <ola/client/StreamingClient.h>
#include <array>
#include <set>
#include "dmx.h"
#include "global.h"

namespace {

const int TCOUNT = 69;
const int BCOUNT = 126;
const int GCOUNT = 106;

// irritating: x-y nature is reversed into row-col here

typedef int LetterArray[TBGB_YMAX][LETTER_WIDTH];

const LetterArray Tdef = {
    { 46, 47, 50, 51, 56, 57, 62, 63, 68 },
    { 45, 48, 49, 52, 55, 58, 61, 64, 67 },
    { 44, 43, 42, 53, 54, 59, 60, 65, 66 },
    { -1, -1, -1, 41, 40, 39, -1, -1, -1 },
    { -1, -1, -1, 36, 37, 38, -1, -1, -1 },
    { -1, -1, -1, 35, 34, 33, -1, -1, -1 },
    { -1, -1, -1, 30, 31, 32, -1, -1, -1 },
    { -1, -1, -1, 29, 28, 27, -1, -1, -1 },
    { -1, -1, -1, 24, 25, 26, -1, -1, -1 },
    { -1, -1, -1, 23, 22, 21, -1, -1, -1 },
    { -1, -1, -1, 18, 19, 20, -1, -1, -1 },
    { -1, -1, -1, 17, 16, 15, -1, -1, -1 },
    { -1, -1, -1, 12, 13, 14, -1, -1, -1 },
    { -1, -1, -1, 11, 10,  9, -1, -1, -1 },
    { -1, -1, -1,  6,  7,  8, -1, -1, -1 },
    { -1, -1, -1,  5,  4,  3, -1, -1, -1 },
    { -1, -1, -1,  0,  1,  2, -1, -1, -1 }
};

const LetterArray Bdef = {
    { 118, 119, 120, 121, 122, 123, 124, 125,  -1 },
    { 117, 116, 115, 114, 113, 112, 111, 110, 109 },
    {  88,  89,  90,  91,  92,  93,  94, 107, 108 },
    {  87,  86,  85,  -1,  -1,  -1,  95, 106, 105 },
    {  82,  83,  84,  -1,  -1,  -1,  96, 103, 104 },
    {  81,  80,  79,  -1,  -1,  -1,  97, 102, 101 },
    {  76,  77,  78,  -1,  -1,  -1,  98,  99, 100 },
    {  75,  74,  73,  72,  71,  70,  69,  68,  67 },
    {  59,  60,  61,  62,  63,  64,  65,  66,  -1 },
    {  58,  57,  44,  43,  42,  41,  40,  39,  38 },
    {  55,  56,  45,  -1,  -1,  -1,  35,  36,  37 },
    {  54,  53,  46,  -1,  -1,  -1,  34,  33,  32 },
    {  51,  52,  47,  -1,  -1,  -1,  29,  30,  31 },
    {  50,  49,  48,  -1,  -1,  -1,  28,  27,  26 },
    {  17,  18,  19,  20,  21,  22,  23,  24,  25 },
    {  16,  15,  14,  13,  12,  11,  10,   9,   8 },
    {   0,   1,   2,   3,   4,   5,   6,   7,  -1 }
};

const LetterArray Gdef = {
    {  -1,  86,  87,  88,  89,  90,  91,  92,  93 },
    {  83,  84,  85,  99,  98,  97,  96,  95,  94 },
    {  82,  81,  80, 100, 101, 102, 103, 104, 105 },
    {  77,  78,  79,  -1,  -1,  -1,  -1,  -1,  -1 },
    {  76,  75,  74,  -1,  -1,  -1,  -1,  -1,  -1 },
    {  71,  72,  73,  -1,  -1,  -1,  -1,  -1,  -1 },
    {  70,  69,  68,  -1,  -1,  -1,  -1,  -1,  -1 },
    {  65,  66,  67,  -1,  -1,  -1,   0,   1,   2 },
    {  64,  63,  62,  -1,  -1,  -1,   5,   4,   3 },
    {  59,  60,  61,  -1,  -1,  -1,   6,   7,   8 },
    {  58,  57,  56,  -1,  -1,  -1,  11,  10,   9 },
    {  53,  54,  55,  -1,  -1,  -1,  12,  13,  14 },
    {  52,  51,  50,  -1,  -1,  -1,  17,  16,  15 },
    {  47,  48,  49,  -1,  -1,  -1,  18,  19,  20 },
    {  46,  45,  44,  43,  42,  41,  40,  39,  21 },
    {  31,  32,  33,  34,  35,  36,  37,  38,  22 },
    {  -1,  30,  29,  28,  27,  26,  25,  24,  23 }
};

const LetterArray* letters[] = { &Tdef, &Bdef, &Gdef, &Bdef };
const int pixcounts[] = { TCOUNT, BCOUNT, GCOUNT, BCOUNT };

void check_grids()
{
    // check to make sure data structures aren't typoed
    std::set<int> check;
    for (auto row = 0; row < TBGB_YMAX; row++)
    {
        for (auto col = 0; col < LETTER_WIDTH; col++)
        {
            auto v = Tdef[row][col];
            if (v == -1)
                continue;
            if (v < -1 || v >= TCOUNT)
                throw std::logic_error("unexpected value in T " + std::to_string(v));
            if (check.find(v) != check.end())
                throw std::logic_error("found duplicate in T " + std::to_string(v));
            check.insert(v);
        }
    }
    for (auto i = 0; i < TCOUNT; i++)
        if (check.find(i) == check.end())
            throw std::logic_error("missing value in T " + std::to_string(i));
    check.clear();
    for (auto row = 0; row < TBGB_YMAX; row++)
    {
        for (auto col = 0; col < LETTER_WIDTH; col++)
        {
            auto v = Bdef[row][col];
            if (v == -1)
                continue;
            if (v < -1 || v >= BCOUNT)
                throw std::logic_error("unexpected value in B " + std::to_string(v));
            if (check.find(v) != check.end())
                throw std::logic_error("found duplicate in B " + std::to_string(v));
            check.insert(v);
        }
    }
    for (auto i = 0; i < BCOUNT; i++)
        if (check.find(i) == check.end())
            throw std::logic_error("missing value in B " + std::to_string(i));
    check.clear();
    for (auto row = 0; row < TBGB_YMAX; row++)
    {
        for (auto col = 0; col < LETTER_WIDTH; col++)
        {
            auto v = Gdef[row][col];
            if (v == -1)
                continue;
            if (v < -1 || v >= GCOUNT)
                throw std::logic_error("unexpected value in G " + std::to_string(v));
            if (check.find(v) != check.end())
                throw std::logic_error("found duplicate in G " + std::to_string(v));
            check.insert(v);
        }
    }
    for (auto i = 0; i < GCOUNT; i++)
        if (check.find(i) == check.end())
            throw std::logic_error("missing value in G " + std::to_string(i));
}
}

DMX::DMX(Framebuf& fb) : m_fb(fb)
{
    check_grids();
    ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR); // stdout not available
    m_buffer.Blackout();
    m_ola_client = new ola::client::StreamingClient(ola::client::StreamingClient::Options());
    if (!m_ola_client->Setup())
        throw std::runtime_error("Could not setup ola client");
}

DMX::~DMX()
{
    delete m_ola_client;
}

void
DMX::render(void)
{
    auto start = std::chrono::system_clock::now();

    const unsigned CHAN_RED = 0;
    const unsigned CHAN_GREEN = 1;
    const unsigned CHAN_BLUE = 2;

    std::lock_guard<std::mutex> lock(m_fb.mutex()); // TODO: finer grain on the mutex? also in viz then?
    for (unsigned universe = 0; universe < 4; universe++) // universe = letter
    {
        m_buffer.Blackout();
        int x0 = universe * (LETTER_WIDTH + 1);      // x start of this letter in Framebuf
        for (int x = x0; x < x0 + LETTER_WIDTH; x++) // x and y travel in Framebuf space
        {
            for (int y = 0; y < TBGB_YMAX; y++)
            {
                int pixel = (*letters[universe])[y][x - x0]; // adjust back to letter space for x
                if (pixel == -1)
                    continue;
                if (pixel < -1 || pixel > pixcounts[universe])
                    throw std::logic_error("bad pixel reverse: u=" + std::to_string(universe) + " x=" + std::to_string(x) 
                        + " y=" + std::to_string(y) + " pix=" + std::to_string(pixel));

                uint8_t red = 255 * m_fb.data(x, y).red;
                uint8_t green = 255 * m_fb.data(x, y).green;
                uint8_t blue = 255 * m_fb.data(x, y).blue;

                m_buffer.SetChannel(pixel * 3 + CHAN_RED, red);
                m_buffer.SetChannel(pixel * 3 + CHAN_GREEN, green);
                m_buffer.SetChannel(pixel * 3 + CHAN_BLUE, blue);

                //std::cout << "map: " << x << "," << y << "->" << pixel << " (" << (unsigned)red << "," 
                //          << (unsigned)green << "," << (unsigned)blue << ")" << std::endl;
            }
        }
        //std::cout << "sending universe " << universe << std::endl;
        // TODO: figure out the blinking
        m_ola_client->SendDmx(universe, m_buffer);
    }

    // timing
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "elapsed DMX::render time: " << diff.count() << std::endl;
}
