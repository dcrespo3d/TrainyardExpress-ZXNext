#ifndef __train_h__
#define __train_h__

#include "types.h"

void train_clear_all();

void train_add(ubyte color, ubyte i, ubyte j, ubyte exit);

void train_update(uword frame);

void train_display_debug_info();

extern uword train_abs_frame;
extern uword train_steps;

// void train_example_draw_many(ubyte frame031);

#endif // __train_h__
