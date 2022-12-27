#include <arch/zxn.h>
#include <z80.h>
#include <input.h>
#include <stdlib.h>

#include "types.h"


////////////////////////////////////////////////////
// this block here for measuring code start location
// dummydummer function must be here
ubyte dummydummer() { return 0; }
extern ubyte start_of_code;
__asm
_start_of_code:
    DEFB 0
__endasm
////////////////////////////////////////////////////


#include "types.h"

#include <input.h>


#include <stdlib.h>
// #include "zxlib/keyb.h"
// #include "zxlib/keyb.c"

#define WAIT_FOR_SCANLINE(line) { \
    while (ZXN_READ_REG(REG_ACTIVE_VIDEO_LINE_L) == line); \
    while (ZXN_READ_REG(REG_ACTIVE_VIDEO_LINE_L) != line); } 

#include "params.h"
#include "train.h"
#include "tilesymbols.h"
#include "colors.h"
#include "spmgr.h"
#include "level.h"
#include "board.h"
#include "design.h"
#include "print_tile.h"
#include "mouse.h"
#include "keyb.h"
#include "joystick.h"
#include "gui.h"
#include "io.h"
#include "screen_controller.h"
#include "screen_tests.h"
#include "screen_main.h"
#include "screen_provs.h"
#include "screen_levels.h"
#include "screen_game.h"
#include "screen_epilog.h"
#include "player_data.h"
#include "page0.h"
#include "sound.h"
#include <string.h>

__asm
EXTERN load_static_sprites
EXTERN setup_tilemap
EXTERN setup_sprites
__endasm;

int main(void)
{
    // CPU SPEED
    // register $07, bits 1-0
    // %00 = 3.5MHz
    // %01 = 7MHz
    // %10 = 14MHz
    // %11 = 28MHz (works since core 3.0)
    //__asm__("NEXTREG $07, %00");
__asm
    LD  A, $07
    CALL ReadNextReg
    AND A, %11111100
    OR  A, %00000011
    NEXTREG $07, A
__endasm;

    zx_border(INK_BLACK);

#define WAIT_ON_START
#ifdef  WAIT_ON_START
    uword waitframes = 50;
    while (waitframes--) {
        WAIT_FOR_SCANLINE(254);
        WAIT_FOR_SCANLINE(255);
    }
#endif

    // initialize player data, loading while splash is still visible
    pd_init();

    // sprites must be loaded first, before 0x4000-0x7FFF gets overwritten
    __asm__("CALL load_static_sprites");

    // level_pagein_levels();

    __asm__("CALL setup_tilemap");

    __asm__("CALL setup_sprites");

    spmgr_init();

    keyb_init();
    joystick_init();
    mouse_init();

    snd_init();

    sc_switch_screen(si_enter, si_update, NULL);
    // sc_switch_screen(so_enter, so_update, NULL);
    // sc_switch_screen(sm_enter, sm_update, NULL);
    // sc_switch_screen(sp_enter, sp_update, NULL);
    // sc_switch_screen(se_enter, se_update, NULL);
    // sc_switch_screen(sa_enter, sa_update, NULL);
    // sg_level_idx = 54;
    // sc_switch_level(sg_level_idx);
    // sc_switch_screen(sta_enter, sta_update, NULL);
    // sc_switch_screen(sv_enter, sv_update, NULL);
    // sc_switch_screen(sw_enter, sw_update, NULL);

    while(1) {
        joystick_update();
        // joystick_print();
        keyb_update();
        // keyb_print();
        mouse_update();

        // update sound
        snd_update();

        // update screen controller
        sc_update();

        WAIT_FOR_SCANLINE(255);
    }

    //NEXTREG(0x15, 0b00010011);

}

// pd_init in player_data.c should be in $8000 - $9FFF slot
#include "player_data.c"
#include "train.c"
#include "board.c"
#include "spmgr.c"
#include "level.c"
#include "design.c"
#include "print_tile.c"
#include "mouse.c"
#include "keyb.c"
#include "joystick.c"
#include "gui.c"
#include "io.c"
#include "screen_controller.c"
#include "screen_tests.c"
#include "screen_main.c"
#include "screen_provs.c"
#include "screen_levels.c"
#include "screen_game.c"
#include "screen_epilog.c"
#include "sound.c"
// #include "page0.c"

__asm

#include "params.asm"
#include "setup_tilemap.asm"
#include "setup_sprites.asm"
#include "trackdefs.asm"
#include "train_sprites_def.asm"
#include "levels.asm"

__endasm;

