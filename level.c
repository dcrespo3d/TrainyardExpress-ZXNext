#include "level.h"
#include "types.h"
#include <string.h>
#include "params.h"
#include "tilesymbols.h"
#include "board.h"
#include "spmgr.h"
#include "train.h"
#include "colors.h"
#include <string.h>

SrcSnk lev_sources[MAX_LEV_SOURCES];
SrcSnk lev_sinks  [MAX_LEV_SINKS];
SrcSnk lev_paints [MAX_LEV_PAINTS];
Obst   lev_obsts  [MAX_LEV_OBSTS];
ubyte  lev_colors [MAX_LEV_COLORS];
ubyte  lev_num_sources = 0;
ubyte  lev_num_sinks = 0;
ubyte  lev_num_paints = 0;
ubyte  lev_num_obsts = 0;
ubyte  lev_num_colors = 0;

ubyte  lev_prov_idx;
ubyte  lev_stars;
ubyte  lev_name   [MAX_LEV_NAME_LEN];

ubyte  train_spr_idx;

// void level_pagein_levels()
// {
// __asm
// 	; load bank/page for train sprite data
//  	NEXTREG $50, 34
// 	NEXTREG $51, 35
// __endasm
// }

ubyte* levrec;

void level_load(ubyte secnum, ubyte levnum)
{
    level_init();

    levrec = levarr[levnum];

    level_parse_levdef();

    board_clear(board);

    spmgr_clear_level();

    level_populate_fixed();
}

void level_init()
{
    lev_num_sources = 0;
    lev_num_sinks = 0;
    lev_num_paints = 0;
    lev_num_obsts = 0;
    lev_num_colors = 0;
    lev_stars = 0;
    memset(lev_sources, 0, MAX_LEV_SOURCES * sizeof(SrcSnk));
    memset(lev_sinks,   0, MAX_LEV_SINKS   * sizeof(SrcSnk));
    memset(lev_paints,  0, MAX_LEV_PAINTS  * sizeof(SrcSnk));
    memset(lev_obsts,   0, MAX_LEV_OBSTS   * sizeof(Obst  ));
    memset(lev_colors,  0, MAX_LEV_COLORS  * sizeof(ubyte ));
    memset(lev_name,    0, MAX_LEV_NAME_LEN* sizeof(ubyte ));
}

ubyte* level_parse_ptr;

void level_parse_levdef()
{
    level_parse_ptr = levrec;

    lev_stars = *level_parse_ptr++;
    lev_prov_idx = *level_parse_ptr++;

    ubyte* name_ptr = &lev_name[0];

    ubyte ch;
    do {
        ch = *level_parse_ptr++;
        *name_ptr++ = ch;
    }
    while (ch != 0);

    level_parse_rawdef();
}

void level_parse_rawdef()
{
    ubyte token = *level_parse_ptr++;
    while (token != 0)
    {
        ubyte type = HINIB(token);
        ubyte dir  = LONIB(token);

        switch(type) {
            case SRC: {
                SrcSnk* src = &lev_sources[lev_num_sources++];
                level_parse_srcsnk(src, dir);
                break;
            }

            case SNK: {
                SrcSnk* snk = &lev_sinks[lev_num_sinks++];
                level_parse_srcsnk(snk, dir);
                break;
            }

            case PNT: {
                SrcSnk* pnt = &lev_paints[lev_num_paints++];
                level_parse_srcsnk(pnt, dir);
                break;                
            }

            case OBS: {
                level_parse_obst();
                break;
            }
        }
        token = *level_parse_ptr++;
    }
}

void level_parse_srcsnk(SrcSnk* srcsnk, ubyte dir)
{
    srcsnk->dir = dir;

    ubyte token = *level_parse_ptr++;
    ubyte i = HINIB(token);
    ubyte j = LONIB(token);

    srcsnk->i = i;
    srcsnk->j = j;

    token = *level_parse_ptr++;
    ubyte ncols = HINIB(token);
    ubyte col0  = LONIB(token);

    srcsnk->colidx = lev_num_colors;
    srcsnk->colcnt = ncols;

    level_parse_colors(col0, ncols);

}

void level_parse_colors(ubyte col0, ubyte ncols)
{
    lev_colors[lev_num_colors++] = col0;
    ncols--;

    ubyte token = 0x77;
    ubyte col;
    while(ncols) {
        token = *level_parse_ptr++;

        lev_colors[lev_num_colors++] = HINIB(token);
        ncols--;

        if (!ncols) break;

        lev_colors[lev_num_colors++] = LONIB(token);
        ncols--;
    }
}


void level_parse_obst()
{
    ubyte token = *level_parse_ptr++;
    ubyte i = HINIB(token);
    ubyte j = LONIB(token);

    // ubyte tdef = JOINIB(OBS, 0);
    // board_set_tile(i, j, tdef);

    Obst* obs = &lev_obsts[lev_num_obsts];
    obs->i = i;
    obs->j = j;

    lev_num_obsts++;
}

void level_populate_fixed()
{
    for (ubyte i = 0; i < lev_num_sources; i++) {
        SrcSnk* src = &lev_sources[i];
        ubyte tdef = JOINIB(SRC, i);
        board_set_tile(src->i, src->j, tdef);
        src->spridx = spmgr_curr_idx;
        spmgr_put_src(src->i, src->j, src->colcnt);
        spmgr_update_srcsnk(src->spridx, src->colidx, src->colcnt);
        spmgr_put_snk_src_mask(src->i, src->j);
    }

    for (ubyte i = 0; i < lev_num_sinks; i++) {
        SrcSnk* snk = &lev_sinks[i];
        ubyte tdef = JOINIB(SNK, i);
        board_set_tile(snk->i, snk->j, tdef);
        snk->chkidx = spmgr_curr_idx;
        spmgr_put_check(snk->i, snk->j);
        spmgr_hide_check(snk->chkidx);
        snk->spridx = spmgr_curr_idx;
        spmgr_put_snk(snk->i, snk->j, snk->colcnt);
        spmgr_update_srcsnk(snk->spridx, snk->colidx, snk->colcnt);
        spmgr_put_snk_src_mask(snk->i, snk->j);
    }

    for (ubyte i = 0; i < lev_num_paints; i++) {
        SrcSnk* pnt = &lev_paints[i];
        ubyte tdef = JOINIB(PNT, i);
        board_set_tile(pnt->i, pnt->j, tdef);
        pnt->spridx = spmgr_curr_idx;
        spmgr_put_pnt(pnt->i, pnt->j, pnt->colidx);
        spmgr_put_snk_src_mask(pnt->i, pnt->j);
    }

    for (ubyte i = 0; i < lev_num_obsts; i++) {
        Obst* obs = &lev_obsts[i];
        ubyte tdef = JOINIB(OBS, 0);
        board_set_tile(obs->i, obs->j, tdef);
    }

    train_spr_idx = spmgr_curr_idx;
}

uword level_frame = 0;

void level_start_trains()
{
    if (level_state & LEV_STATE_RUNNING)
        return;
    level_set_state(LEV_STATE_RUNNING);
    level_frame = 0;
    train_steps = 0;
    train_clear_all();
}

void level_stop_trains()
{
    level_set_state(LEV_STATE_DESIGN);
}

sbyte level_speed = 0;
sbyte level_prev_speed = 0;
ubyte level_slow = 0;
ubyte level_slow_target = 0;
ubyte level_frame_inc = 1;

void level_set_speed(sbyte speed)
{
    if (speed < -6) speed = -6;
    if (speed >  5) speed =  5;
    level_speed = speed;
    if (speed == 0) {
        level_slow_target = 0;
        level_frame_inc = 1;
        return;
    }
    if (speed <= 0) {
        level_frame_inc = 1;
        level_slow_target = 1 << -(1 + speed);
        return;
    } 
    if (speed >= 0) {
        level_slow_target = 0;
        level_frame_inc = 1 << speed;
    }
}

void level_update()
{
    if (!(level_state & LEV_STATE_RUNNING))
        return;

    if (level_slow_target > 0)
    {
        if (level_slow_target > 16) return;
        if (level_slow++ < level_slow_target)
            return;
        else
            level_slow = 0;
    }

    train_update(level_frame);

    level_frame += level_frame_inc;
}

ubyte level_sink_matches_color(SrcSnk* snk, ubyte color)
{
    for (ubyte i = 0; i < snk->colcnt; i++) {
        if (lev_colors[snk->colidx + i] == color) {
            return 1;
        }
    }
    return 0;
}

void level_sink_remove_color(SrcSnk* snk, ubyte color)
{
    for (ubyte i = 0; i < snk->colcnt; i++) {
        if (lev_colors[snk->colidx + i] == color) {
            lev_colors[snk->colidx + i] = COLOR_BLK;
            spmgr_update_srcsnk(snk->spridx, snk->colidx, snk->colcnt);
            return;
        }
    }
}

void level_sink_check_all_done(SrcSnk* snk)
{
    for (ubyte i = 0; i < snk->colcnt; i++) {
        if (lev_colors[snk->colidx + i] != COLOR_BLK)
            return;
    }

    spmgr_show_check(snk->chkidx);
}

///////////////////////////////////////////////////////////////////////////////

#include "screen_game.h"

ubyte level_state = LEV_STATE_IDLE;

void level_set_state(ubyte state)
{
    if (state == level_state) return;
    level_state = state;
}
