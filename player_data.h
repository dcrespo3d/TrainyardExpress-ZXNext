#ifndef __player_data_h__
#define __player_data_h__

#include "types.h"

extern uword pd_score;
extern ubyte pd_levels_solved[];

extern ubyte pd_all_levels_unlocked;

#define STARS_IN_BASE_LEVELS 267
#define STARS_IN_BONUS_LEVELS 397
extern ubyte pd_base_levels_solved;
extern ubyte pd_bonus_levels_solved;

void pd_init();
void pd_update_score();

void pd_save_levels_solved();

void pd_save_level_board(ubyte levidx, ubyte* srcboard);
void pd_load_level_board(ubyte levidx, ubyte* dstboard);

// utility (fast ASM)
void memory_clear_slot(ubyte slotnum);

void pd_clear_undo();
void pd_snapshot();
void pd_undo();

extern ubyte pd_sens;
void pd_set_sens(ubyte sens);

extern ubyte pd_debug_on;

#endif // __player_data_h__
