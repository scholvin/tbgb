#pragma once

static const int LETTER_WIDTH = 9;

static const int T1_START = 0;
static const int B2_START = T1_START + LETTER_WIDTH + 1;
static const int G3_START = B2_START + LETTER_WIDTH + 1;
static const int B4_START = G3_START + LETTER_WIDTH + 1;
static const int TBGB_XMAX = B4_START + LETTER_WIDTH;;
static const int TBGB_YMAX = 17;

bool is_visible(int x, int y);
