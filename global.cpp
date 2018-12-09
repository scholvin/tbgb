#include <array>
#include "global.h"

namespace {
// x dimension is 1 larger to account for null-terminated string
const std::array<std::array<char, TBGB_XMAX+1>, TBGB_YMAX> visible = {
    "XXXXXXXXX XXXXXXXX   XXXXXXXX XXXXXXXX ",
    "XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX",
    "XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX",
    "   XXX    XXX   XXX XXX       XXX   XXX",
    "   XXX    XXX   XXX XXX       XXX   XXX",
    "   XXX    XXX   XXX XXX       XXX   XXX",
    "   XXX    XXX   XXX XXX       XXX   XXX",
    "   XXX    XXXXXXXXX XXX   XXX XXXXXXXXX",
    "   XXX    XXXXXXXX  XXX   XXX XXXXXXXX ",
    "   XXX    XXXXXXXXX XXX   XXX XXXXXXXXX",
    "   XXX    XXX   XXX XXX   XXX XXX   XXX",
    "   XXX    XXX   XXX XXX   XXX XXX   XXX",
    "   XXX    XXX   XXX XXX   XXX XXX   XXX",
    "   XXX    XXX   XXX XXX   XXX XXX   XXX",
    "   XXX    XXXXXXXXX XXXXXXXXX XXXXXXXXX",
    "   XXX    XXXXXXXXX XXXXXXXXX XXXXXXXXX",
    "   XXX    XXXXXXXX   XXXXXXXX XXXXXXXX "
};
}

bool is_visible(int x, int y)
{
    if (x < 0 || x >= TBGB_XMAX || y < 0 || y >= TBGB_YMAX)
        return false;

    return (visible[y][x] == 'X'); // looks backwards but is ok
}
