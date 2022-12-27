#include "screen_provs.h"
#include "page0.h"

extern void p0_sp_enter();
extern void p0_sp_update();

void sp_enter()
{
    p0_pagein();
    p0_sp_enter();
}

void sp_update()
{
    p0_pagein();
    p0_sp_update();
}

extern void p0_sb_enter();
extern void p0_sb_update();

void sb_enter()
{
    p0_pagein();
    p0_sb_enter();
}

void sb_update()
{
    p0_pagein();
    p0_sb_update();
}

