#include "train.h"
#include "types.h"
#include "params.h"
#include "board.h"
#include "level.h"
#include "tilesymbols.h"
#include "colors.h"
#include "spmgr.h"
#include "sound.h"

ubyte train_idx, train_col, train_i, train_j, train_frame;

static ubyte* spr_def_ptr;
static ubyte spr_pat, spr_w, spr_h, spr_dx, spr_dy;

extern ubyte train_sprites_def;

#define MIRR_X  0b00001000
#define MIRR_Y  0b00000100
#define R90_CW  0b00000010

#define TSF_X   0b00001000
#define TSF_Y   0b00000100
#define TSF_R   0b00000010
#define TSF_XY  0b00001100
#define TSF_YR  0b00000110
#define TSF_XR  0b00001010
#define TSF_XYR 0b00001110

// #asm
// _spr_pat: defb 0
// _spr_w:   defb 0
// _spr_h:   defb 0
// _spr_dx:  defb 0
// _spr_dy:  defb 0
// #endasm

void loadSpriteDefForFrame()
{
    spr_def_ptr = &train_sprites_def;
    spr_def_ptr += 5 * train_frame;
    // #asm
    //     LD HL, _train_sprites_def
    //     LD A, (_train_frame)
    //     LD B, A
    //     RLCA
    //     RLCA
    //     AND A, %11111100
    //     ADD A, B
    //     LD C, A
    //     LD B, 0
    //     ADD HL, BC
    //     LD (_spr_def_ptr), HL
    // #endasm
}

void loadSpriteDefForFrame0()
{
    spr_def_ptr = &train_sprites_def;
}

void loadSpriteData()
{
#define LOADSPRITEDATA_USE_ASM
#ifndef LOADSPRITEDATA_USE_ASM
    spr_pat    = *spr_def_ptr++;
    spr_w      = *spr_def_ptr++;
    spr_h      = *spr_def_ptr++;
    spr_dx     = *spr_def_ptr++;
    spr_dy     = *spr_def_ptr++;

    param0b = train_spr_idx + 2 * train_idx;
    param1b = spr_pat;
    param2b = (BOARD_X + 4 * train_i) * 8;
    param3b = (BOARD_Y + 4 * train_j) * 8;
    param4b = (train_col << 4);
    param5b = spr_w - 16;
    param6b = spr_h - 16;
#else
__asm
    // spr_pat    = *spr_def_ptr++;
    // spr_w      = *spr_def_ptr++;
    // spr_h      = *spr_def_ptr++;
    // spr_dx     = *spr_def_ptr++;
    // spr_dy     = *spr_def_ptr++;
    LD HL, (_spr_def_ptr)

    LD A,(HL)
    LD (_spr_pat), A
    INC HL

    LD A,(HL)
    LD (_spr_w), A
    INC HL

    LD A,(HL)
    LD (_spr_h), A
    INC HL

    LD A,(HL)
    LD (_spr_dx), A
    INC HL

    LD A,(HL)
    LD (_spr_dy), A
    INC HL

    LD (_spr_def_ptr), HL

    // param0b = train_spr_idx + 2 * train_idx;
    LD A, (_train_idx)
    RLCA
    LD B, A
    LD A, (_train_spr_idx);
    ADD A, B
    LD (_param0b), A

    // param1b = spr_pat;
    LD A, (_spr_pat)
    LD (_param1b), A

    // param2b = (BOARD_X + 4 * train_i) * 8;
    LD A, (_train_i)
    RLCA
    RLCA
    ADD A, BOARD_X
    RLCA
    RLCA
    RLCA
    LD (_param2b), A

    // param3b = (BOARD_Y + 4 * train_j) * 8;
    LD A, (_train_j)
    RLCA
    RLCA
    ADD A, BOARD_Y
    RLCA
    RLCA
    RLCA
    LD (_param3b), A

    // param4b = (train_col << 4);
    LD A, (_train_col)
    SWAPNIB
    AND A, %11110000
    LD (_param4b), A

    // param5b = spr_w - 16;
    LD A, (_spr_w)
    SUB A, 16
    LD (_param5b), A

    // param6b = spr_h - 16;
    LD A, (_spr_h)
    SUB A, 16
    LD (_param6b), A

__endasm


#endif // LOADSPRITEDATA_USE_ASM
}

// prototypes
void train_animate_wn();
void train_animate_es();
void train_animate_ne();
void train_animate_sw();
void train_animate_ws();
void train_animate_en();
void train_animate_se();
void train_animate_nw();

void train_animate_ns();
void train_animate_sn();
void train_animate_we();
void train_animate_ew();

void train_animate_xw();
void train_animate_xe();
void train_animate_xn();
void train_animate_xs();

void train_animate_wx();
void train_animate_ex();
void train_animate_nx();
void train_animate_sx();

#define train_animate_ASM
#ifndef train_animate_ASM

///////////////////////////////////////////////////////////////////////////////
// curved segments
///////////////////////////////////////////////////////////////////////////////

void train_animate_wn()
{
    loadSpriteDefForFrame();
    loadSpriteData();

    param2b += spr_dx - 32;
    param3b += spr_dy - 32;

    __asm__("CALL setup_2x1_sprite");
}

void train_animate_es()
{
    loadSpriteDefForFrame();
    loadSpriteData();

    param2b += 48 - spr_dx;
    param3b += 48 - spr_dy;
    param4b |= TSF_XY;

    __asm__("CALL setup_2x1_sprite");
}

void train_animate_ne()
{
    loadSpriteDefForFrame();
    loadSpriteData();

    param2b += 48 - spr_dy;
    param3b += spr_dx - 32;
    param4b |= TSF_R;

    __asm__("CALL setup_2x1_sprite");
}

void train_animate_sw()
{
    loadSpriteDefForFrame();
    loadSpriteData();

    param2b += spr_dy - 32;
    param3b += 48 - spr_dx;
    param4b |= TSF_XYR;

    __asm__("CALL setup_2x1_sprite");
}

void train_animate_ws()
{
    loadSpriteDefForFrame();
    loadSpriteData();

    param2b += spr_dx - 32;
    param3b += 48 - spr_dy;
    param4b |= TSF_Y;

    __asm__("CALL setup_2x1_sprite");
}

void train_animate_en()
{
    loadSpriteDefForFrame();
    loadSpriteData();

    param2b += 48 - spr_dx;
    param3b += spr_dy - 32;
    param4b |= TSF_X;

    __asm__("CALL setup_2x1_sprite");
}

void train_animate_se()
{
    loadSpriteDefForFrame();
    loadSpriteData();

    param2b += 48 - spr_dy;
    param3b += 48 - spr_dx;
    param4b |= TSF_YR;

    __asm__("CALL setup_2x1_sprite");
}

void train_animate_nw()
{
    loadSpriteDefForFrame();
    loadSpriteData();

    param2b += spr_dy - 32;
    param3b += spr_dx - 32;
    param4b |= TSF_XR;

    __asm__("CALL setup_2x1_sprite");
}

///////////////////////////////////////////////////////////////////////////////
// straight segments
///////////////////////////////////////////////////////////////////////////////

void train_animate_ns()
{
    loadSpriteDefForFrame0();
    loadSpriteData();

    param2b += spr_dy - 32;
    param3b += spr_dx - 32 + train_frame;
    param4b |= TSF_XR;

    __asm__("CALL setup_2x1_sprite");
}

void train_animate_sn()
{
    loadSpriteDefForFrame0();
    loadSpriteData();

    param2b += spr_dy - 32;
    param3b += spr_dx + 16 - train_frame;
    param4b |= TSF_XYR;

    __asm__("CALL setup_2x1_sprite");
}

void train_animate_we()
{
    loadSpriteDefForFrame0();
    loadSpriteData();

    param2b += spr_dx - 32 + train_frame;
    param3b += spr_dy - 32;
    __asm__("CALL setup_2x1_sprite");
}

void train_animate_ew()
{
    loadSpriteDefForFrame0();
    loadSpriteData();

    param2b += spr_dx + 16 - train_frame;
    param3b += spr_dy - 32;
    param4b |= TSF_X;

    __asm__("CALL setup_2x1_sprite");
}

#else
__asm
#include  "train_animate.asm"
__endasm
#endif

///////////////////////////////////////////////////////////////////////////////
// source segments
///////////////////////////////////////////////////////////////////////////////

void train_animate_xw()
{
    if (train_frame < 16) train_frame = 16;
    train_animate_ew();
}

void train_animate_xe()
{
    if (train_frame < 16) train_frame = 16;
    train_animate_we();
}

void train_animate_xn()
{
    if (train_frame < 16) train_frame = 16;
    train_animate_sn();
}

void train_animate_xs()
{
    if (train_frame < 16) train_frame = 16;
    train_animate_ns();
}

///////////////////////////////////////////////////////////////////////////////
// sink segments
///////////////////////////////////////////////////////////////////////////////

void train_animate_wx()
{
    if (train_frame > 16) train_frame = 16;
    train_animate_we();
}

void train_animate_ex()
{
    if (train_frame > 16) train_frame = 16;
    train_animate_ew();
}

void train_animate_nx()
{
    if (train_frame > 16) train_frame = 16;
    train_animate_ns();
}

void train_animate_sx()
{
    if (train_frame > 16) train_frame = 16;
    train_animate_sn();
}

///////////////////////////////////////////////////////////////////////////////
// MACROS
///////////////////////////////////////////////////////////////////////////////

#define TRAIN_ANIMATE_wn(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_wn(); }

#define TRAIN_ANIMATE_ne(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_ne(); }

#define TRAIN_ANIMATE_sw(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_sw(); }

#define TRAIN_ANIMATE_es(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_es(); }

#define TRAIN_ANIMATE_ws(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_ws(); }

#define TRAIN_ANIMATE_en(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_en(); }

#define TRAIN_ANIMATE_se(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_se(); }

#define TRAIN_ANIMATE_nw(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_nw(); }

#define TRAIN_ANIMATE_ns(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_ns(); }

#define TRAIN_ANIMATE_sn(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_sn(); }

#define TRAIN_ANIMATE_ew(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_ew(); }

#define TRAIN_ANIMATE_we(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_we(); }

#define TRAIN_ANIMATE_xw(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_xw(); }

#define TRAIN_ANIMATE_xs(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_xs(); }

#define TRAIN_ANIMATE_xe(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_xe(); }

#define TRAIN_ANIMATE_xn(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_xn(); }

#define TRAIN_ANIMATE_wx(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_wx(); }

#define TRAIN_ANIMATE_sx(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_sx(); }

#define TRAIN_ANIMATE_ex(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_ex(); }

#define TRAIN_ANIMATE_nx(TRIDX,TRCOL,TILEI,TILEJ,FRAME) { train_idx = TRIDX; train_col = TRCOL; train_i = TILEI; train_j = TILEJ; train_frame = FRAME; \
train_animate_nx(); }

///////////////////////////////////////////////////////////////////////////////

// void train_example_draw_many(ubyte frame031)
// {
//     TRAIN_ANIMATE_wn( 0, 1, 3, 3, frame031);
//     TRAIN_ANIMATE_ne( 1, 1, 1, 3, frame031);
//     TRAIN_ANIMATE_sw( 2, 1, 3, 1, frame031);
//     TRAIN_ANIMATE_es( 3, 1, 1, 1, frame031);

//     TRAIN_ANIMATE_ns( 4, 1, 1, 2, frame031);
//     TRAIN_ANIMATE_sn( 5, 1, 3, 2, frame031);
//     TRAIN_ANIMATE_ew( 6, 1, 2, 1, frame031);
//     TRAIN_ANIMATE_we( 7, 1, 2, 3, frame031);

//     TRAIN_ANIMATE_ws( 8, 2, 5, 3, frame031);
//     TRAIN_ANIMATE_en( 9, 2, 3, 5, frame031);
//     TRAIN_ANIMATE_se(10, 2, 3, 3, frame031);
//     TRAIN_ANIMATE_nw(11, 2, 5, 5, frame031);

//     TRAIN_ANIMATE_sn(12, 2, 3, 4, frame031);
//     TRAIN_ANIMATE_ns(13, 2, 5, 4, frame031);
//     TRAIN_ANIMATE_we(14, 2, 4, 3, frame031);
//     TRAIN_ANIMATE_ew(15, 2, 4, 5, frame031);

//     TRAIN_ANIMATE_xw(16, 3, 6, 0, frame031);
//     TRAIN_ANIMATE_xs(17, 4, 5, 0, frame031);
//     TRAIN_ANIMATE_xe(18, 3, 5, 1, frame031);
//     TRAIN_ANIMATE_xn(19, 4, 6, 1, frame031);

//     TRAIN_ANIMATE_wx(20, 5, 1, 6, frame031);
//     TRAIN_ANIMATE_sx(21, 6, 1, 5, frame031);
//     TRAIN_ANIMATE_ex(22, 5, 0, 5, frame031);
//     TRAIN_ANIMATE_nx(23, 6, 0, 6, frame031);
// }

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    ubyte color;
    ubyte i;
    ubyte j;
    ubyte pi;
    ubyte pj;
    ubyte idir;
    ubyte odir;
    ubyte flags;
} Train;

#define TRAIN_MASK_FINISHED     0b00000001
#define TRAIN_MASK_NEG_FINISHED 0b11111110
#define TRAIN_MASK_CRASHED      0b00000010
#define TRAIN_MASK_NEG_CRASHED  0b11111101
#define TRAIN_MASK_COMBINED     0b00000100
#define TRAIN_MASK_NEG_COMBINED 0b11111011
#define TRAIN_MASK_CRA_FIN_COM  0b00000111

#define MAX_TRAINS 16

Train train_array[MAX_TRAINS];

#include <string.h>

// integer time
// happens at 'integer' frames: 0, 32, 64, 96, 128...
uword itime;
uword itime_prev;

// half time
// happens at 'integer+half' frames: 16, 48, 80, 112...
uword htime;
uword htime_prev;

// fractional time [0,31]
ubyte ftime;

void train_clear_all()
{
    memset(train_array, 0, MAX_TRAINS * sizeof(Train));
}

typedef void (*Train1Callback)(ubyte idx, Train* train);

void train_all_active(Train1Callback cb1)
{
    for (ubyte idx = 0; idx < MAX_TRAINS; idx++)
    {
        Train* train = &(train_array[idx]);
        if (train->color == 0) continue;    // loop active trains only
        cb1(idx, train);
    }
}

typedef void (*Train2Callback)(Train* train1, Train* train2);

void train_all_active_pairs(Train2Callback cb2)
{
    for (ubyte j = 0; j < MAX_TRAINS; j++)
    {
        Train* train1 = &(train_array[j]);
        if (train1->color == 0) continue;   // loop active trains only
        for (ubyte i = j+1; i < MAX_TRAINS; i++)
        {
            Train* train2 = &(train_array[i]);
            if (train2->color == 0) continue;   // loop active trains only
            cb2(train1, train2);
        }
    }
}

void train_add(ubyte color, ubyte i, ubyte j, ubyte odir)
{
    if (color == 0) return;
    if (color >= 8) return;

    for (ubyte idx = 0; idx < MAX_TRAINS; idx++) {
        Train* train = &(train_array[idx]);
        if (train->color != 0) continue;

        // empty slot found
        train->color = color;
        train->i = i;
        train->j = j;
        train->pi = 7;  // avoid collision at start
        train->pj = 7;
        train->idir = 0;
        train->odir = odir;
        train->flags = 0;

        break;
    }
}

void train_delete_unneeded(ubyte idx, Train* train)
{
    if (train->flags & TRAIN_MASK_CRA_FIN_COM) {
        if (train->flags & TRAIN_MASK_FINISHED) {
            ubyte tdef = board_get_tile(train->i, train->j);
            ubyte type = HINIB(tdef);
            if (type == SNK) {
                ubyte idx  = LONIB(tdef);
                SrcSnk* snk = &(lev_sinks[idx]);
                level_sink_remove_color(snk, train->color);
                level_sink_check_all_done(snk);
            }
        }
        train->color = 0;
        param0b = train_spr_idx + 2 * idx;
        __asm__("CALL disable_sprite");
    }
}

void train_switch_track(ubyte idx, Train* train);
void train_advance(ubyte idx, Train* train);
void train_animate(ubyte idx, Train* train);
void train_check_paint(ubyte idx, Train* train);

void train_check_join(Train* train1, Train* train2);
void train_check_collision(Train* train1, Train* train2);
void train_check_half_crocol(Train* train1, Train* train2);

void train_check_finished();

uword train_abs_frame = 0;
uword train_steps = 0;

// #define TRAIN_DISPLAY_DEBUG_INFO

void train_update(uword frame)
{
    if (frame == 0) {
        itime_prev = 0xFFFF;
        htime_prev = 0xFFFF;
    }

    itime = frame >> 5;
    ftime = frame & 0b11111;
    htime = (frame - 16) >> 5;

    train_abs_frame = frame;

    if (htime_prev != htime) {
        htime_prev  = htime;

        // checks performed at half time
        // (in the middle of a tile)

        // check cross / collision events
        train_all_active_pairs(train_check_half_crocol);

        // check painter tile
        train_all_active(train_check_paint);

#ifdef TRAIN_DISPLAY_DEBUG_INFO
        train_array[14].flags++;
        train_array[14].flags &= 0xF;
        train_display_debug_info();
#endif
    }

    if (itime_prev != itime) {
        itime_prev  = itime;

        // increase train steps (itime eventually overflows)
        train_steps++;

        // checks performed at integer time
        // (in the boundaries of a tile)

        // perform track switch when a train leaves its tile
        train_all_active(train_switch_track);

        // join trains when both enter the same tile from the same side
        train_all_active_pairs(train_check_join);

        // delete trains marked for deletion
        train_all_active(train_delete_unneeded);

        // advance trains: calculate it next route
        train_all_active(train_advance);

        // check for boundary collision
        train_all_active_pairs(train_check_collision);

        // spawn new trains from source
        for (ubyte idx = 0; idx < MAX_LEV_SOURCES; idx++) {
            SrcSnk* src = &(lev_sources[idx]);
            if (itime < src->colcnt) {
                ubyte color = lev_colors[src->colidx + itime];
                train_add(color, src->i, src->j, src->dir);
            }
            if (itime > 0 && itime <= src->colcnt) {
                lev_colors[src->colidx + itime - 1] = COLOR_BLK;
                spmgr_update_srcsnk(src->spridx, src->colidx, src->colcnt);
            }
        }

        // delete trains marked for deletion
        train_all_active(train_delete_unneeded);

        // check if all trains got into sinks and no trains crashed
        train_check_finished();

#ifdef TRAIN_DISPLAY_DEBUG_INFO
        train_array[15].flags++;
        train_array[15].flags &= 0xF;
        train_display_debug_info();
#endif

    }

    // put animation frame for active trains
    train_all_active(train_animate);
}

void train_animate(ubyte idx, Train* train)
{
    train_idx = idx;
    train_col = train->color;
    train_i = train->i;
    train_j = train->j;
    train_frame = ftime;

    void (*fun_ptr)() = train_animate_xn;

    switch(train->idir) {
        case XX:
            switch(train->odir) {
                case DIRN: fun_ptr = train_animate_xn; break;
                case DIRW: fun_ptr = train_animate_xw; break;
                case DIRS: fun_ptr = train_animate_xs; break;
                case DIRE: fun_ptr = train_animate_xe; break;
            }
            break;
        case DIRN:
            switch(train->odir) {
                // case DIRN: fun_ptr = train_animate_nn; break;
                case DIRW: fun_ptr = train_animate_nw; break;
                case DIRS: fun_ptr = train_animate_ns; break;
                case DIRE: fun_ptr = train_animate_ne; break;
                case   XX: fun_ptr = train_animate_nx; break;
            }
            break;
        case DIRW:
            switch(train->odir) {
                case DIRN: fun_ptr = train_animate_wn; break;
                // case DIRW: fun_ptr = train_animate_ww; break;
                case DIRS: fun_ptr = train_animate_ws; break;
                case DIRE: fun_ptr = train_animate_we; break;
                case   XX: fun_ptr = train_animate_wx; break;
            }
            break;
        case DIRS:
            switch(train->odir) {
                case DIRN: fun_ptr = train_animate_sn; break;
                case DIRW: fun_ptr = train_animate_sw; break;
                // case DIRS: fun_ptr = train_animate_ss; break;
                case DIRE: fun_ptr = train_animate_se; break;
                case   XX: fun_ptr = train_animate_sx; break;
            }
            break;
        case DIRE:
            switch(train->odir) {
                case DIRN: fun_ptr = train_animate_en; break;
                case DIRW: fun_ptr = train_animate_ew; break;
                case DIRS: fun_ptr = train_animate_es; break;
                // case DIRE: fun_ptr = train_animate_ee; break;
                case   XX: fun_ptr = train_animate_ex; break;
            }
            break;
    }

    (*fun_ptr)();
}

void train_switch_track(ubyte idx, Train* train)
{
    // prev train state
    ubyte pi = train->i;
    ubyte pj = train->j;

    // prev tile value
    ubyte pval = board_get_tile(pi, pj);
    ubyte pv0 = pval & 0xF;
    ubyte pv1 = pval >> 4;

    if (pv0 == pv1)
        return;

    // switch prev track
    if (tile_is_track(pval)) {
        pval = pv1 | (pv0 << 4);
        board_set_tile(pi, pj, pval);
        board_draw_tile(pi, pj, pval);

        if (tile_has3exits(pval))
            snd_play_switch();
    }
}

void train_advance(ubyte idx, Train* train)
{
    // p for prev, n for next

    // prev train state
    ubyte pid = train->idir;
    ubyte pod = train->odir;
    ubyte pi = train->i;
    ubyte pj = train->j;
    ubyte c = train->color;

    // prev tile value
    ubyte pval = board_get_tile(pi, pj);

    if (tile_is_sink(pval) && pod == XX) {
        train->flags |= TRAIN_MASK_FINISHED;    // finished = true
        snd_play_color(train->color);
        return;
    }

    // next tile coordinate
    ubyte ni = pi;
    ubyte nj = pj;
    if      (pod == DIRN) nj--;
    else if (pod == DIRW) ni--;
    else if (pod == DIRS) nj++;
    else if (pod == DIRE) ni++;

    // next entry direction
    ubyte nid = pid;
    ubyte nod = pod;
    if      (pod == DIRN) nid = DIRS;
    else if (pod == DIRW) nid = DIRE;
    else if (pod == DIRS) nid = DIRN;
    else if (pod == DIRE) nid = DIRW;

    // next tile value
    ubyte nval = board_get_tile(ni, nj);
    if (tile_is_track(nval)) {
        ubyte nv0 = nval & 0x0F;
        ubyte nv1 = nval >> 4;

        // calculate output direction
        if      (nv0 & nid) nod = nv0 & ~nid;
        else if (nv1 & nid) nod = nv1 & ~nid;
        else {  train->flags |= TRAIN_MASK_CRASHED;     // crashed = true;
            level_set_state(LEV_STATE_CRASHED);
            snd_play_crash();
        }
    }
    else if (tile_is_sink(nval)) {
        ubyte idx = LONIB(nval);
        SrcSnk* snk = &(lev_sinks[idx]);
        ubyte dir = snk->dir;
        if (!(dir&nid)) {
            train->flags |= TRAIN_MASK_CRASHED;     // crashed = true;
            level_set_state(LEV_STATE_CRASHED);
            snd_play_crash();
        }
        else if (!level_sink_matches_color(snk, train->color)) {
            train->flags |= TRAIN_MASK_CRASHED;     // crashed = true;
            level_set_state(LEV_STATE_CRASHED);
            snd_play_crash();
        }
        nod = XX;
    }
    else if (tile_is_paint(nval)) {
        ubyte idx = LONIB(nval);
        SrcSnk* pnt = &(lev_paints[idx]);
        ubyte dir = pnt->dir;
        // calculate output direction
        if      (dir & nid) nod = dir & ~nid;
        else {  train->flags |= TRAIN_MASK_CRASHED;     // crashed = true;
            level_set_state(LEV_STATE_CRASHED);
            snd_play_crash();
        }
    }
    else {
        train->flags |= TRAIN_MASK_CRASHED;     // crashed = true;
        level_set_state(LEV_STATE_CRASHED);
        snd_play_crash();
    }

    train->idir = nid;
    train->odir = nod;
    train->i = ni;
    train->j = nj;
    train->pi = pi;
    train->pj = pj;
}

ubyte train_color_combine(ubyte col1, ubyte col2)
{
    if (col1 == col2) return col1;
    if (col1 & col2) return COLOR_BRO;
    return (col1 | col2);
}

void train_check_join(Train* train1, Train* train2)
{
    if (train1->i != train2->i) return;
    if (train1->j != train2->j) return;
    if (train1->odir != train2->odir) return;
    if (train1->odir == XX) return; // do not join at sink

    ubyte color = train_color_combine(train1->color, train2->color);
    snd_play_color(color);
    train1->color = color;
    train2->color = color;
    train2->flags |= TRAIN_MASK_COMBINED;
}

void train_check_collision(Train* train1, Train* train2)
{
    if (train1->pi != train2->i) return;
    if (train1->pj != train2->j) return;
    if (train2->pi != train1->i) return;
    if (train2->pj != train1->j) return;

    ubyte color = train_color_combine(train1->color, train2->color);
    snd_play_color(color);
    train1->color = color;
    train2->color = color;
}

ubyte train_is_crossover(ubyte idir1, ubyte odir1, ubyte idir2, ubyte odir2)
{
    ubyte mask = 0;
    mask |= idir1;
    mask |= odir1;
    if (mask != 0x5 && mask != 0xA) // NS, WE
        return 0;
    mask |= idir2;
    mask |= odir2;
    if (mask != 0xF) return 0; // NWSE

    return 1;
}

ubyte train_is_collision(ubyte idir1, ubyte odir1, ubyte idir2, ubyte odir2)
{
    if (idir1 != odir2) return 0;
    if (idir2 != odir1) return 0;
    return 1;
}

void train_check_half_crocol(Train* train1, Train* train2)
{
    if (train1->i != train2->i) return;
    if (train1->j != train2->j) return;

    ubyte crossover = train_is_crossover(
        train1->idir, train1->odir, train2->idir, train2->odir);
    
    ubyte collision = train_is_collision(
        train1->idir, train1->odir, train2->idir, train2->odir);
    
    if (!crossover && !collision) return;

    ubyte color = train_color_combine(train1->color, train2->color);
    snd_play_color(color);
    train1->color = color;
    train2->color = color;
}

void train_check_paint(ubyte idx, Train* train)
{
    ubyte tdef = board_get_tile(train->i, train->j);
    if (!tile_is_paint(tdef)) return;

    ubyte idx = LONIB(tdef);
    SrcSnk* pnt = &(lev_paints[idx]);
    train->color = lev_colors[pnt->colidx];
    snd_play_color(train->color);
}

void train_check_finished()
{
    // level is not finished if any train has crashed
    if (level_state == LEV_STATE_CRASHED)
        return;

    // level is not finished if any sink has a missing train
    for (ubyte i = 0; i < lev_num_sinks; i++) {
        SrcSnk* snk = &(lev_sinks[i]);
        for (ubyte j = 0; j < snk->colcnt; j++) {
            ubyte color = lev_colors[snk->colidx + j];
            if (color != COLOR_BLK)
                return;
        }
    }
        
    for (ubyte i = 0; i < MAX_TRAINS; i++) {
        Train* train = &(train_array[i]);
        if (train->color == 0) continue;
        if (!(train->flags & TRAIN_MASK_FINISHED))
            return;
    }
    level_set_state(LEV_STATE_FINISHED);
}

#include "print_tile.h"

void train_display_debug_info()
{
    #define TDDI_X0 24
    print_set_pos(TDDI_X0, 0);

    for (ubyte i = 0; i < MAX_TRAINS; i++) {
        Train* train = &(train_array[i]);
        print_set_color(8 + train->color);
        ubyte tidx = '0' + train->flags;
        if (train->flags > 9) tidx += 7;
        print_char(tidx);
    }

}