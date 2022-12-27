#ifndef __joystick_h__
#define __joystick_h__

#include "types.h"

extern ubyte joyLeft;  // left
extern ubyte joyRight; // right
extern ubyte joyUp;    // up
extern ubyte joyDown;  // down
extern ubyte joyBut1;  // button 1 (left)
extern ubyte joyBut2;  // button 2 (right)

extern ubyte joydata;  // raw joystick data

void joystick_init();

void joystick_update();

void joystick_print();

#endif