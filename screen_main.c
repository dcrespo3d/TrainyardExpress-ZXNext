#include "screen_main.h"
#include "page0.h"
#include "page1.h"

extern void p0_sm_enter();
extern void p0_sm_update();

void sm_enter()
{
    p0_pagein();
    p0_sm_enter();
}

void sm_update()
{
    p0_pagein();
    p0_sm_update();
}

extern void p0_sa_enter();
extern void p0_sa_update();

void sa_enter()
{
    p0_pagein();
    p0_sa_enter();
}

void sa_update()
{
    p0_pagein();
    p0_sa_update();
}

extern void p0_so_enter();
extern void p0_so_update();

void so_enter()
{
    p0_pagein();
    p0_so_enter();
}

void so_update()
{
    p0_pagein();
    p0_so_update();
}

extern void p0_si_enter();
extern void p0_si_update();

void si_enter()
{
    p0_pagein();
    p0_si_enter();
}

void si_update()
{
    p0_pagein();
    p0_si_update();
}

extern void p1_st_enter();
extern void p1_st_update();

void stut_enter()
{
    p1_pagein();
    p1_st_enter();
}

void stut_update()
{
    p1_pagein();
    p1_st_update();
}

