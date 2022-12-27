#ifndef __mouse_h__
#define __mouse_h__

#include "types.h"

// mouse coordinates
// ranging from 0 to 319 (x)
//              0 to 255 (y)
extern uword mouse_x;
extern ubyte mouse_y;

// mouse half coordinates
// ranging from 0 to 159 (x)
//              0 to 127 (y)
extern ubyte mouse_hx;
extern ubyte mouse_hy;

// mouse button instantaneous state
extern ubyte mouse_but_l;
extern ubyte mouse_but_r;

// convenience: button was pressed in this frame
extern ubyte mouse_pressed_l;
extern ubyte mouse_pressed_r;

// convenience: button was released in this frame
extern ubyte mouse_released_l;
extern ubyte mouse_released_r;

// convenience: button was double clicked in this frame
extern ubyte mouse_dblclick_l;
extern ubyte mouse_dblclick_r;

void mouse_init();
void mouse_update();

typedef void (*MouseAction)();
extern MouseAction mouseAction;
void nullMouseAction();

void mouse_print_debug();

#endif // __mouse_h__
