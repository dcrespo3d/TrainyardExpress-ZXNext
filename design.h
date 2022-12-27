#ifndef _DESIGN_H
#define _DESIGN_H

#include "types.h"

void design_init();

void design_update();

void design_print_debug();

void design_update_fake();

extern ubyte fake_hx;
extern ubyte fake_hy;
extern ubyte fake_pressed_l;
extern ubyte fake_but_l;
extern ubyte fake_dblclick_l;
extern ubyte fake_released_l;
extern ubyte fake_pressed_r;
extern ubyte fake_but_r;
extern ubyte fake_released_r;

#endif