#include "spmgr.h"
#include "types.h"
#include "params.h"
#include "board.h"
#include "colors.h"
#include "level.h"
#include "print_tile.h"

#define SPR_PAT_SRC_SNK_MASK 64
#define SPR_PAT_SRC_1  65
#define SPR_PAT_SRC_4  66
#define SPR_PAT_SNK_1  67
#define SPR_PAT_SNK_4  68
#define SPR_PAT_TLMARK 69
#define SPR_PAT_CURSOR 70
#define SPR_PAT_HAND   71
#define SPR_PAT_TALLY  72
#define SPR_PAT_PAINT  73
#define SPR_PAT_CHECK  77
#define SPR_PAT_SRC_9  81
#define SPR_PAT_SNK_9  82

ubyte spmgr_curr_idx = 0;
ubyte spmgr_notfix_idx = 0;

ubyte spmgr_cursor_idx = 0;
ubyte spmgr_marker_idx = 0;

ubyte spmgr_fakcur_idx = 0;

__asm
EXTERN clear_sprites
EXTERN init_cursor_sprite
EXTERN init_2x2_quad_sprite
EXTERN update_2x2_quad_sprite_palettes
EXTERN update_2x2_quad_sprite_hide
EXTERN update_2x2_quad_sprite_show
EXTERN update_2x2_quad_sprite_pos
EXTERN update_cursor_sprite
EXTERN setup_scale2x_sprite
EXTERN setup_2x2_sprite
EXTERN update_2x2_sprite_palette
__endasm;

void spmgr_init()
{
    param0b = 0;
    __asm__("CALL clear_sprites");
    spmgr_curr_idx = 0;
}

///////////////////////////////////////////////////////////////////////////////

void spmgr_setup_cursor()
{
    spmgr_cursor_idx = spmgr_curr_idx;
    spmgr_cursor_set_arrow();
    spmgr_curr_idx += 1;
}

sbyte spmgr_cursor_dx = 0;

void spmgr_cursor_set_arrow()
{
    param0b = spmgr_cursor_idx;   // sprite
    param1b = SPR_PAT_CURSOR;     // pattern
    __asm__("CALL init_cursor_sprite");
    spmgr_cursor_dx = 0;
}

void spmgr_cursor_set_hand()
{
    param0b = spmgr_cursor_idx;   // sprite
    param1b = SPR_PAT_HAND;       // pattern
    __asm__("CALL init_cursor_sprite");
    spmgr_cursor_dx = -8;
}

void spmgr_update_cursor(uword x, ubyte y)
{
    param0b = spmgr_cursor_idx;
    param0w = x + spmgr_cursor_dx;
    param1b = y;
    __asm__("CALL update_cursor_sprite");
}

///////////////////////////////////////////////////////////////////////////////

void spmgr_setup_fake_cursor()
{
    spmgr_fakcur_idx = spmgr_curr_idx;
    param0b = spmgr_fakcur_idx;   // sprite
    param1b = SPR_PAT_HAND;       // pattern
    __asm__("CALL init_cursor_sprite");
    spmgr_curr_idx += 1;
}

void spmgr_update_fake_cursor(uword x, ubyte y)
{
    param0b = spmgr_fakcur_idx;
    param0w = x - 8;
    param1b = y;
    __asm__("CALL update_cursor_sprite");
}

///////////////////////////////////////////////////////////////////////////////

void spmgr_setup_tally(uword x, ubyte y, ubyte attr)
{
    param0b = spmgr_curr_idx;   // sprite
    param1b = SPR_PAT_TALLY;       // pattern
    __asm__("CALL init_cursor_sprite");

    param0w = x;
    param1b = y;
    param2b = attr;
    __asm__("CALL update_cursor_sprite_ex");
    spmgr_curr_idx += 1;
}

void spmgr_hide_tally(ubyte spridx)
{
    param0b = spridx;   // sprite
    param1b = SPR_PAT_TALLY;       // pattern
    __asm__("CALL hide_cursor_sprite");
}

void spmgr_show_tally(ubyte spridx)
{
    param0b = spridx;   // sprite
    param1b = SPR_PAT_TALLY;       // pattern
    __asm__("CALL show_cursor_sprite");
}

///////////////////////////////////////////////////////////////////////////////

void spmgr_setup_marker()
{
    spmgr_marker_idx = spmgr_curr_idx;
    param0b = spmgr_marker_idx;   // sprite
    param1b = SPR_PAT_TLMARK;     // pattern
    param2b = 8; // (BOARD_X + 4*i) * 0;  // x
    param3b = 24; // (BOARD_Y + 4*j) * 0;  // y
    __asm__("CALL init_2x2_quad_sprite");
    param4b = param5b = param6b = param7b = 0;
    __asm__("CALL update_2x2_quad_sprite_palettes");
    spmgr_curr_idx += 4;
}

void spmgr_hide_marker()
{
    param0b = spmgr_marker_idx;   // sprite
    param1b = SPR_PAT_TLMARK;     // pattern
    __asm__("CALL update_2x2_quad_sprite_hide");
}

void spmgr_show_marker()
{
    param0b = spmgr_marker_idx;   // sprite
    param1b = SPR_PAT_TLMARK;     // pattern
    __asm__("CALL update_2x2_quad_sprite_show");
}

void spmgr_pos_marker(ubyte x, ubyte y)
{
    param0b = spmgr_marker_idx;   // sprite
    param2b = x;
    param3b = y;
    __asm__("CALL update_2x2_quad_sprite_pos");
}

///////////////////////////////////////////////////////////////////////////////
 
void spmgr_end_fixed_sprites()
{
    spmgr_notfix_idx = spmgr_curr_idx;
}

///////////////////////////////////////////////////////////////////////////////

void spmgr_clear_level()
{
    param0b = spmgr_notfix_idx;
    __asm__("CALL clear_sprites");
    spmgr_curr_idx = spmgr_notfix_idx;
}

///////////////////////////////////////////////////////////////////////////////

void spmgr_put_snk_src_mask(ubyte i, ubyte j)
{
    param0b = spmgr_curr_idx;   // sprite
    param1b = 64;                   // pattern
    param2b = (BOARD_X + 4*i) * 8;  // x
    param3b = (BOARD_Y + 4*j) * 8;  // y
    __asm__("CALL setup_scale2x_sprite");
    spmgr_curr_idx += 1;
}

void spmgr_put_src(ubyte i, ubyte j, ubyte colcnt)
{
    if (colcnt <= 4) {
        param0b = spmgr_curr_idx;   // sprite
        param1b = SPR_PAT_SRC_1;                   // pattern
        if (colcnt > 1)
            param1b = SPR_PAT_SRC_4;
        param2b = (BOARD_X + 4*i) * 8;  // x
        param3b = (BOARD_Y + 4*j) * 8;  // y
        __asm__("CALL init_2x2_quad_sprite");
        spmgr_curr_idx += 4;
    }
    else {
        ubyte x0 = 5 + (BOARD_X + 4*i) * 8;  // x
        ubyte y0 = 5 + (BOARD_Y + 4*j) * 8;  // y

        for (ubyte j = 0; j < 3; j++) {
            for (ubyte i = 0; i < 3; i++) {
                param0b = spmgr_curr_idx++;
                param1b = SPR_PAT_SRC_9;
                param2b = x0 + 7*i;
                param3b = y0 + 7*j;
                __asm__("CALL init_src9snk_sprite");
            }
        }
    }
}

void spmgr_put_snk(ubyte i, ubyte j, ubyte colcnt)
{
    if (colcnt <= 4) {
        param0b = spmgr_curr_idx;   // sprite
        param1b = SPR_PAT_SNK_1;                   // pattern
        if (colcnt > 1)
            param1b = SPR_PAT_SNK_4;
        param2b = (BOARD_X + 4*i) * 8;  // x
        param3b = (BOARD_Y + 4*j) * 8;  // y
        __asm__("CALL init_2x2_quad_sprite");
        spmgr_curr_idx += 4;
    }
    else {
        ubyte x0 = 5 + (BOARD_X + 4*i) * 8;  // x
        ubyte y0 = 5 + (BOARD_Y + 4*j) * 8;  // y

        for (ubyte j = 0; j < 3; j++) {
            for (ubyte i = 0; i < 3; i++) {
                param0b = spmgr_curr_idx++;
                param1b = SPR_PAT_SNK_9;
                param2b = x0 + 7*i;
                param3b = y0 + 7*j;
                __asm__("CALL init_src9snk_sprite");
            }
        }
    }
}

void spmgr_update_srcsnk(ubyte spridx, ubyte colidx, ubyte colcnt)
{
    if (colcnt <= 4) {
        param0b = spridx;   // sprite
        if (colcnt == 1) {
            ubyte color = lev_colors[colidx];
            param4b = color;
            param5b = color;
            param6b = color;
            param7b = color;
        }
        else {
            param4b = lev_colors[colidx];
            param5b = lev_colors[colidx+1];
            param6b = COLOR_BLK;
            param7b = COLOR_BLK;
            if (colcnt > 2) param6b = lev_colors[colidx+2];
            if (colcnt > 3) param7b = lev_colors[colidx+3];
        }
        __asm__("CALL update_2x2_quad_sprite_palettes");
    }
    else {
        for (ubyte i = 0; i < 9; i++) {
            param0b = spridx++;
            if (i < colcnt)
                param4b = lev_colors[colidx + i];
            else
                param4b = COLOR_BLK;
            __asm__("CALL update_src9snk_sprite_color");
        }
    }
}


void spmgr_put_src_1(ubyte i, ubyte j, ubyte color)
{
    param0b = spmgr_curr_idx;   // sprite
    param1b = 65;                   // pattern
    param2b = (BOARD_X + 4*i) * 8;  // x
    param3b = (BOARD_Y + 4*j) * 8;  // y
    param4b = color;
    param5b = color;
    param6b = color;
    param7b = color;
    __asm__("CALL init_2x2_quad_sprite");
    __asm__("CALL update_2x2_quad_sprite_palettes");
    spmgr_curr_idx += 4;
}

void spmgr_put_snk_1(ubyte i, ubyte j, ubyte color)
{
    param0b = spmgr_curr_idx;   // sprite
    param1b = 67;                   // pattern
    param2b = (BOARD_X + 4*i) * 8;  // x
    param3b = (BOARD_Y + 4*j) * 8;  // y
    param4b = color;
    param5b = color;
    param6b = color;
    param7b = color;
    __asm__("CALL init_2x2_quad_sprite");
    __asm__("CALL update_2x2_quad_sprite_palettes");
    spmgr_curr_idx += 4;
}

void spmgr_put_pnt(ubyte i, ubyte j, ubyte colidx)
{
    param0b = spmgr_curr_idx;       // sprite
    param1b = SPR_PAT_PAINT;        // pattern
    param2b = (BOARD_X + 4*i) * 8;  // x
    param3b = (BOARD_Y + 4*j) * 8;  // y
    __asm__("CALL setup_2x2_sprite");
    spmgr_curr_idx += 4;

    // using text colors
    param4b = 8 + lev_colors[colidx];
    __asm__("CALL update_2x2_sprite_palette");
}



void spmgr_put_check(ubyte i, ubyte j)
{
    param0b = spmgr_curr_idx;   // sprite
    param1b = SPR_PAT_CHECK;                   // pattern
    param2b = (BOARD_X + 4*i) * 8;  // x
    param3b = (BOARD_Y + 4*j) * 8;  // y
    __asm__("CALL setup_2x2_sprite");
    spmgr_curr_idx += 4;
}

void spmgr_set_check_color(ubyte spridx, ubyte color)
{
    param0b = spridx;   // sprite
    param4b = color;
    __asm__("CALL update_2x2_sprite_palette");
}

void spmgr_hide_check(ubyte spridx)
{
    param0b = spridx;             // sprite
    param1b = SPR_PAT_TLMARK;     // pattern
    __asm__("CALL update_2x2_quad_sprite_hide");
}

void spmgr_show_check(ubyte spridx)
{
    param0b = spridx;             // sprite
    param1b = SPR_PAT_CHECK;     // pattern
    __asm__("CALL update_2x2_quad_sprite_show");
}

///////////////////////////////////////////////////////////////////////////////

void spmgr_print_debug()
{
#define spmgr_print_debug_ENABLED
    ubyte x = 30;
    ubyte y = 2;
    print_set_color(CTEXT_WHI);

    print_set_pos(x, y);
    print_str("SPRITE MGR");
#ifndef  spmgr_print_debug_ENABLED
    print_set_pos(x, ++y);
    print_str("DISABLED");
#else
    y += 2;
    x++;

    print_set_pos(x, y++); print_str("CURSOR "); print_hex_byte(spmgr_cursor_idx);
    print_set_pos(x, y++); print_str("MARKER "); print_hex_byte(spmgr_marker_idx);
    print_set_pos(x, y++); print_str("NOTFIX "); print_hex_byte(spmgr_notfix_idx);
    y++;
    print_set_pos(x, y++); print_str("CURRNT "); print_hex_byte(spmgr_curr_idx);
#endif
}
