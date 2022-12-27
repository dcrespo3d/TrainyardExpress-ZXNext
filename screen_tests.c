#include "screen_tests.h"

extern void p0_st_enter();
extern void p0_st_update();

void st_enter()
{
    p0_pagein();
    p0_st_enter();
}

void st_update()
{
    p0_pagein();
    p0_st_update();
}

extern void p0_sta_enter();
extern void p0_sta_update();

void sta_enter()
{
    p0_pagein();
    p0_sta_enter();
}

void sta_update()
{
    p0_pagein();
    p0_sta_update();
}

extern void p0_stk_enter();
extern void p0_stk_update();

void stk_enter()
{
    p0_pagein();
    p0_stk_enter();
}

void stk_update()
{
    p0_pagein();
    p0_stk_update();
}

extern void p0_sts_enter();
extern void p0_sts_update();

void sts_enter()
{
    p0_pagein();
    p0_sts_enter();
}

void sts_update()
{
    p0_pagein();
    p0_sts_update();
}

