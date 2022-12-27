#ifndef __page0_h__
#define __page0_h__

void p0_pagein()
{
    __asm__("NEXTREG $50, 36");
    __asm__("NEXTREG $51, 37");
}

#endif