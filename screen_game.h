#ifndef __screen_game_h__
#define __screen_game_h__

#include "types.h"

void sg_enter();
void sg_update();

void sg_draw_score();

extern ubyte sg_level_idx;
extern ubyte sg_prov_idx;
extern ubyte sg_score_earned;

#endif // __screen_game_h__
