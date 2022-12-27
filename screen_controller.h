#ifndef __screen_controller_h__
#define __screen_controller_h__

#include <stddef.h>
#include "types.h"

// FunPtr is a pointer to a function accepting no arguments
// and returning void
typedef void (*FunPtr)();

void sc_switch_delay(ubyte nframes);

void sc_switch_screen(FunPtr entry, FunPtr update, FunPtr exit);

void sc_switch_level(ubyte index);

ubyte sc_is_level_unlocked(ubyte level_idx);

void sc_switch_next_level();

void sc_update();

#endif // __screen_controller_h__
