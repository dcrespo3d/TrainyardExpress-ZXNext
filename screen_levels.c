#include "screen_levels.h"

extern void p0_sl_enter();
extern void p0_sl_update();

void sl_enter()
{
    p0_pagein();
    p0_sl_enter();
}

void sl_update()
{
    p0_pagein();
    p0_sl_update();
}

