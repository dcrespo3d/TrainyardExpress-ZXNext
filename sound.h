#ifndef __sound_h__
#define __sound_h__

#include "types.h"

void snd_init();
void snd_update();

// stop all sound
void snd_panic();

void snd_play_color(ubyte color);
void snd_play_tone(uword period);

void snd_play_crash();
void snd_play_switch();
void snd_play_success();

ubyte snd_get_ay_reg8(ubyte reg);
void snd_put_ay_reg8(ubyte reg, ubyte val);
void snd_put_ay_reg16(ubyte reg, uword val);
void snd_select_ay(ubyte idx);


#endif // __sound_h__
