#ifndef __page1_h__
#define __page1_h__

void p1_pagein()
{
    __asm__("NEXTREG $50, 38");
    __asm__("NEXTREG $51, 39");
}

#endif