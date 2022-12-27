#include "screen_epilog.h"
#include "page0.h"

extern void p0_se_enter();
extern void p0_se_update();

void se_enter()
{
    p0_pagein();
    p0_se_enter();
}

void se_update()
{
    p0_pagein();
    p0_se_update();
}

extern void p0_sv_enter();
extern void p0_sv_update();

void sv_enter()
{
    p0_pagein();
    p0_sv_enter();
}

void sv_update()
{
    p0_pagein();
    p0_sv_update();
}

extern void p0_sw_enter();
extern void p0_sw_update();

void sw_enter()
{
    p0_pagein();
    p0_sw_enter();
}

void sw_update()
{
    p0_pagein();
    p0_sw_update();
}

