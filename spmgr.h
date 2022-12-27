#ifndef __spmgr_h__
#define __spmgr_h__

#include "types.h"

// sprite manager

extern ubyte spmgr_curr_idx;

// initialize (and cleanup) sprite manager, clearing all sprites
void spmgr_init();

void spmgr_setup_cursor();
void spmgr_update_cursor(uword x, ubyte y);

void spmgr_setup_fake_cursor();
void spmgr_update_fake_cursor(uword x, ubyte y);

void spmgr_setup_tally(uword x, ubyte y, ubyte attr);
void spmgr_hide_tally(ubyte spridx);
void spmgr_show_tally(ubyte spridx);

void spmgr_setup_marker();
void spmgr_hide_marker();
void spmgr_show_marker();
void spmgr_pos_marker(ubyte x, ubyte y);

void spmgr_end_fixed_sprites();

void spmgr_clear_level();

void spmgr_put_snk_src_mask(ubyte i, ubyte j);

void spmgr_put_src(ubyte i, ubyte j, ubyte colcnt);

void spmgr_put_snk(ubyte i, ubyte j, ubyte colcnt);

void spmgr_put_pnt(ubyte i, ubyte j, ubyte colidx);

void spmgr_update_srcsnk(ubyte spridx, ubyte colidx, ubyte colcnt);

void spmgr_put_src_1(ubyte i, ubyte j, ubyte color);

void spmgr_put_snk_1(ubyte i, ubyte j, ubyte color);

void spmgr_put_check(ubyte i, ubyte j);
void spmgr_set_check_color(ubyte spridx, ubyte color);
void spmgr_show_check(ubyte spridx);
void spmgr_hide_check(ubyte spridx);

void spmgr_cursor_set_arrow();
void spmgr_cursor_set_hand();

void spmgr_print_debug();



#endif // __spmgr_h__