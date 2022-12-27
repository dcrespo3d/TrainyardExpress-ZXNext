#include "board.h"
#include "level.h"
#include "tilesymbols.h"
#include "params.h"
#include "print_tile.h"

#include <string.h>

ubyte board[BOARD_SZ];
ubyte board_stored[BOARD_SZ];

void board_clear(ubyte* dst)
{
#define board_clear_ASM
#ifndef board_clear_ASM
    ubyte rem_bytes = BOARD_SZ;
    while (rem_bytes--) {
        *dst = 0;
        dst++;
    }
#else
__asm
    POP BC      ; return address
    POP HL      ; dst
    PUSH HL     ; restore stack
    PUSH BC     ; restore stack

    XOR A
    LD  B, BOARD_SZ
board_clear_loop:
    LD (HL), A
    INC HL
    DJNZ board_clear_loop

__endasm
#endif
}

void board_clear_tracks(ubyte* dst)
{
#define board_clear_tracks_ASM
#ifndef board_clear_tracks_ASM
    ubyte rem_bytes = BOARD_SZ;
    while (rem_bytes--) {
        ubyte tval = *dst;
        if (tile_is_track(tval))
            *dst = XXXX;
        dst++;        
    }
#else
__asm
    POP BC      ; return address
    POP HL      ; dst
    PUSH HL     ; restore stack
    PUSH BC     ; restore stack

    LD  B, BOARD_SZ
board_clear_tracks_loop:
    PUSH HL
    LD  L, (HL)
    LD  H, 0
    PUSH HL
    CALL _tile_is_track
    LD  A, L
    OR  A
    POP HL
    POP HL
    JR  Z, board_clear_tracks_skip
    XOR A 
    LD (HL), A
board_clear_tracks_skip:
    INC HL
    DJNZ board_clear_tracks_loop

__endasm
#endif
}

void board_copy(ubyte* src, ubyte* dst)
{
#define board_copy_ASM
#ifndef board_copy_ASM
    ubyte rem_bytes = BOARD_SZ;
    while (rem_bytes--) {
        ubyte* tval = *src;
        *dst = tval;
        dst++;
        src++;
    }
#else
__asm
    POP BC  ; retdir
    POP HL  ; dst
    POP DE  ; src

    PUSH DE
    PUSH HL
    PUSH BC

    LD  B, BOARD_SZ
board_copy_loop:
    LD  A, (DE)
    LD (HL), A
    INC DE
    INC HL
    DJNZ board_copy_loop

__endasm
#endif
}

void board_copy_tracks(ubyte* src, ubyte *dst)
{
#define board_copy_tracks_ASM
#ifndef board_copy_tracks_ASM
    ubyte rem_bytes = BOARD_SZ;
    while (rem_bytes--) {
        ubyte tval = *src;
        if (tile_is_track(tval))
            *dst = tval;
        src++;
        dst++;
    }
#else
__asm
    POP BC  ; retdir
    POP HL  ; dst
    POP DE  ; src

    PUSH DE
    PUSH HL
    PUSH BC

    LD  B, BOARD_SZ
board_copy_tracks_loop:
    PUSH HL
    // PUSH DE
    PUSH BC
    LD  A, (DE)
    LD  L, A
    LD  H, 0
    PUSH HL
    CALL _tile_is_track
    LD  A, L
    OR  A
    POP HL
    POP BC
    // POP DE
    POP HL
    JR  Z, board_copy_tracks_skip
    LD  A, (DE)
    LD (HL), A
board_copy_tracks_skip:
    INC DE
    INC HL
    DJNZ board_copy_tracks_loop

__endasm
#endif
}

// void board_copy_not_fixed(ubyte* src, ubyte* dst)
// {
//     ubyte rem_bytes = BOARD_SZ;
//     while (rem_bytes--) {
//         ubyte tval = *src;
//         if (!tile_is_fixed(tval))
//             *dst = tval;
//         src++;
//         dst++;
//     }
// }

ubyte board_is_equal(ubyte* board1, ubyte* board2)
{
    ubyte rem_bytes = BOARD_SZ;
    while (rem_bytes--) {
        if (*board1 != *board2)
            return 0;
        board1++;
        board2++;
    }
    return 1;
}

extern ubyte trackdefs;

// tilemap start address
ubyte* tmstart = (ubyte*)0x4000;

ubyte bdr_tidx;
ubyte bdr_x0, bdr_y0;
ubyte bdr_i, bdr_j;
ubyte bdr_tval;

#define draw_tile_xy_USE_GLOBAL_VERSION
#ifndef draw_tile_xy_USE_GLOBAL_VERSION

void draw_tile_xy(ubyte ti, ubyte tj, ubyte idx)
{
    ubyte* src = &trackdefs;
    src += (32 * idx);
    
    ubyte* dst;
    for (ubyte j = 0; j < 4; j++) {
        dst = tmstart;
        dst += 80 * (tj + j);
        dst += 2 * ti;
        for (ubyte i = 0; i < 4; i++) {
            *dst++ = *src++;
            *dst++ = *src++;
        }
    }
}

#else

// global parameters:
// bdr_x : x coord of tile
// bdr_y : y coord of tile
// bdr_idx : index in tile definitions
void __draw_tile_xy()
{
#define __draw_tile_xy_USE_ASM
#ifndef __draw_tile_xy_USE_ASM
    ubyte* src = &trackdefs;
    src += (32 * bdr_tidx);
    
    ubyte* dst;
    for (ubyte j = 0; j < 4; j++) {
        dst = tmstart;
        dst += 80 * (bdr_y0 + j);
        dst += 2 * bdr_x0;
        for (ubyte i = 0; i < 4; i++) {
            *dst++ = *src++;
            *dst++ = *src++;
        }
    }
#else
__asm
    LD HL, (_bdr_tidx)
    LD H, 0
    ADD HL, HL
    ADD HL, HL
    ADD HL, HL
    ADD HL, HL
    ADD HL, HL

    LD DE, _trackdefs
    ADD HL, DE

    PUSH HL

    LD B, 0
    __draw_tile_xy_outer:
        LD A, (_bdr_y0)
        ADD A, B
        LD H, 0
        LD L, A
        ; HL = bdr_y0 + j

        ADD HL, HL
        ADD HL, HL
        ADD HL, HL
        ADD HL, HL
        LD DE, HL
        ADD HL, HL
        ADD HL, HL
        ADD HL, DE
        ; HL = 80 * (bdr_y0 + j)

        LD A, (_bdr_x0)
        RLCA
        LD D, 0
        LD E, A
        ; DE = 2 * bdr_x0

        ADD HL, DE
        LD DE, HL
        LD HL, (_tmstart)
        ADD HL, DE
        ; HL = dst = tmstart + (80 * (bdr_y0 + j)) + 2 * bdr_x0;

        POP DE
        ; DE = src

        LD C, 0
        __draw_tile_xy_inner:
            LD A, (DE)
            LD (HL), A
            INC DE
            INC HL

            LD A, (DE)
            LD (HL), A
            INC DE
            INC HL

            INC C
            LD A, 4
            SUB A, C
        
        JP NZ, __draw_tile_xy_inner

        PUSH DE

        INC B
        LD  A, 4
        SUB A, B

    JP NZ, __draw_tile_xy_outer

    POP HL

__endasm
#endif
}

#endif // draw_tile_xy_USE_GLOBAL_VERSION


// params:
// bdr_i : column of tile
// bdr_j : row of tile
// tidx: index of tile to be drawn (unchanged)
void __draw_tile_ij()
{
#define draw_tile_ij_ASM
#ifndef draw_tile_ij_ASM
    bdr_x0 = BOARD_X + 4 * bdr_i;
    bdr_y0 = BOARD_Y + 4 * bdr_j;
    // 
    __draw_tile_xy();
#else
__asm
    LD A, (_bdr_j)
    RLCA
    RLCA
    ADD A, BOARD_Y
    LD (_bdr_y0), A

    LD A, (_bdr_i)
    RLCA
    RLCA
    ADD A, BOARD_X
    LD (_bdr_x0), A

    CALL ___draw_tile_xy

__endasm
#endif
}

void __board_draw_tile();

void board_draw()
{
#define board_draw_ASM
#ifndef board_draw_ASM
    ubyte* defptr = board;
    for (ubyte j = 0; j < BOARD_H; j++) {
        for (ubyte i = 0; i < BOARD_W; i++) {
            ubyte tval = *defptr++;
            board_draw_tile(i, j, tval);
        }
    }
#else
__asm
    LD  HL, _board
    LD  B, 0
board_draw_outer:
    LD  A, B
    LD (_bdr_j), A

    LD  C, 0
board_draw_inner:
    LD  A, C
    LD (_bdr_i), A

    LD  A, (HL)
    INC HL
    LD (_bdr_tval), A

    PUSH HL
    PUSH BC
    CALL ___board_draw_tile
    POP BC
    POP HL

    INC C
    LD  A, BOARD_W
    CP  A, C
    JR NZ, board_draw_inner

    INC B
    LD  A, BOARD_H
    CP  A, B
    JR NZ, board_draw_outer
__endasm
#endif
}

#define draw_tile_ij(idx, i, j) \
{ bdr_tidx = idx; bdr_i = i; bdr_j = j; __draw_tile_ij();}

ubyte bdr_src_table[] = {
    TILE_SRCE,  // 0 (8 & 7)
    TILE_SRCN,  // 1 (1 & 7)
    TILE_SRCW,  // 2 (2 & 7)
    TILE_ERR,
    TILE_SRCS,  // 4 (4 & 7)
};

ubyte bdr_snk_table[] = {
    TILE_ERR,   //
    TILE_SNKN,  // 1
    TILE_SNKW,  // 2
    TILE_SNNW,  // 3
    TILE_SNKS,  // 4
    TILE_SNNS,  // 5
    TILE_SNWS,  // 6
    TILE_KNWS,  // 7
    TILE_SNKE,  // 8
    TILE_SNNE,  // 9
    TILE_SNWE,  // 10
    TILE_KNWE,  // 11
    TILE_SNSE,  // 12
    TILE_KNSE,  // 13
    TILE_KWSE,  // 14
    TILE_SNAL,  // 15
};

void board_draw_tile(ubyte i, ubyte j, ubyte tval)
{
    bdr_i = i;
    bdr_j = j;
    bdr_tval = tval;
    __board_draw_tile();
}

// parameters:
// bdr_i : column for drawing tile
// bdr_j : row for drawing tile
// bdr_tval : tile value to be decoded
void __board_draw_tile()
{
#define __board_draw_tile_ASM
#ifndef __board_draw_tile_ASM
    bdr_tidx = TILE_ERR;

    ubyte hi = HINIB(bdr_tval);
    if (hi == XX) {
        bdr_tidx = TILE_BLNK;
        __draw_tile_ij();
        return;
    }

    if (hi == OBS) {
        bdr_tidx = TILE_OBST;
        __draw_tile_ij();
        return;
    }

    ubyte lo = LONIB(bdr_tval);

    if (hi == SRC) {
        SrcSnk* src = &lev_sources[lo];
        ubyte idx = src->dir & 7;
        bdr_tidx = bdr_src_table[idx];
        __draw_tile_ij();
        return;
    }

    if (hi == SNK) {
        SrcSnk* snk = &lev_sinks[lo];
        bdr_tidx = bdr_snk_table[snk->dir];
        __draw_tile_ij();
        return;
    }

    if (hi == PNT) {
        SrcSnk* pnt = &lev_paints[lo];
        bdr_tidx = bdr_snk_table[pnt->dir];
        __draw_tile_ij();
        return;
    }

    bdr_tidx= ((lo >> 1) - 1) + ((hi >> 1) - 1) * 6;
    __draw_tile_ij();
#else
__asm
    LD  A, TILE_ERR
    LD (_bdr_tidx), A

    LD  A, (_bdr_tval)
    SWAPNIB
    AND A, %00001111
    LD  B, A    ; hi

    CP  A, XX
    JR NZ, __board_draw_tile_NOT_BLNK

    LD  A, TILE_BLNK
    LD (_bdr_tidx), A
    CALL ___draw_tile_ij
    RET

__board_draw_tile_NOT_BLNK:
    LD  A, B
    CP  A, OBS
    JR NZ, __board_draw_tile_NOT_OBST

    LD  A, TILE_OBST
    LD (_bdr_tidx), A
    CALL ___draw_tile_ij
    RET

__board_draw_tile_NOT_OBST:
    LD  A, (_bdr_tval)
    AND A, %00001111
    LD  C, A    ; lo

    LD  A, B
    CP  A, SRC
    JR NZ, __board_draw_tile_NOT_SRC

    LD  A, C    ; lo
    RLCA
    RLCA
    RLCA        ; lo * 8
    LD  D, 0
    LD  E, A
    LD  HL, _lev_sources
    ADD HL, DE

    LD  A, (HL) ; src->dir
    AND A, 7    ; idx = src->dir & 7
    LD  D, 0
    LD  E, A
    LD  HL, _bdr_src_table
    ADD HL, DE
    LD  A, (HL) ; _bdr_src_table[idx]

    LD (_bdr_tidx), A
    CALL ___draw_tile_ij
    RET

__board_draw_tile_NOT_SRC:
    LD  A, B
    CP  A, SNK
    JR NZ, __board_draw_tile_NOT_SNK

    LD  A, C    ; lo
    RLCA
    RLCA
    RLCA        ; lo * 8
    LD  D, 0
    LD  E, A
    LD  HL, _lev_sinks
    ADD HL, DE

    LD  A, (HL) ; idx = snk->dir
    LD  D, 0
    LD  E, A
    LD  HL, _bdr_snk_table
    ADD HL, DE
    LD  A, (HL) ; _bdr_snk_table[idx]

    LD (_bdr_tidx), A
    CALL ___draw_tile_ij
    RET

__board_draw_tile_NOT_SNK:
    LD  A, B
    CP  A, PNT
    JR NZ, __board_draw_tile_NOT_PNT

    LD  A, C    ; lo
    RLCA
    RLCA
    RLCA        ; lo * 8
    LD  D, 0
    LD  E, A
    LD  HL, _lev_paints
    ADD HL, DE

    LD  A, (HL) ; idx = pnt->dir
    LD  D, 0
    LD  E, A
    LD  HL, _bdr_snk_table
    ADD HL, DE
    LD  A, (HL) ; _bdr_snk_table[idx]

    LD (_bdr_tidx), A
    CALL ___draw_tile_ij
    RET

__board_draw_tile_NOT_PNT:
    LD  A, B    ; hi
    RRCA
    AND A, %01111111
    DEC A       ; ((hi >> 1) - 1)

    RLCA
    LD  B, A
    RLCA
    ADD A, B
    LD  B, A    ; ((hi >> 1) - 1) * 6

    LD  A, C    ; lo
    RRCA
    AND A, %01111111
    DEC A       ; ((lo >> 1) - 1)

    ADD A, B    ; ((lo >> 1) - 1) + ((hi >> 1) - 1) * 6
    LD (_bdr_tidx), A
    CALL ___draw_tile_ij
__endasm
#endif
}

ubyte board_get_tile(ubyte i, ubyte j)
{
#define board_get_tile_ASM
#ifndef board_get_tile_ASM
    ubyte idx = i + j * 7;
    return board[idx];
#else
__asm
    LD HL, 2
    ADD HL, SP
    LD  B, (HL)   ; j
    INC HL
    INC HL
    LD  C, (HL)   ; i

    LD  A, B
    RLCA
    RLCA
    RLCA
    SUB A, B
    ADD A, C   ; A = i + j * 7

    LD  D, 0
    LD  E, A

    LD  HL, _board
    ADD HL, DE
    LD  L, (HL)
    LD  H, 0
__endasm
#endif
}

void board_set_tile(ubyte i, ubyte j, ubyte tval)
{
#define board_set_tile_ASM
#ifndef board_set_tile_ASM
    ubyte idx = i + j * 7;
    board[idx] = tval;
#else
__asm
    LD HL, 2
    ADD HL, SP
    LD  E, (HL)   ; tval
    INC HL
    INC HL
    LD  B, (HL)   ; j
    INC HL
    INC HL
    LD  C, (HL)   ; i

    LD  A, B
    RLCA
    RLCA
    RLCA
    SUB A, B
    ADD A, C   ; A = i + j * 7

    LD  B, E   ; tval

    LD  D, 0
    LD  E, A

    LD  HL, _board
    ADD HL, DE
    LD (HL), B
__endasm
#endif
}


ubyte tile_is_sink(ubyte tval)
{
#define tile_is_sink_ASM
#ifndef tile_is_sink_ASM
    ubyte hi = HINIB(tval);
    return hi == SNK;
#else
__asm
    LD  HL, 2
    ADD HL, SP
    LD  A, (HL)   ; tval
    SWAPNIB
    AND A, %00001111
    LD  HL, 1
    CP  A, SNK
    JR  Z, ASMPC+3   ; skip the DEC HL
    DEC HL
__endasm
#endif
}

ubyte tile_is_paint(ubyte tval)
{
#define tile_is_paint_ASM
#ifndef tile_is_paint_ASM
    ubyte hi = HINIB(tval);
    return hi == PNT;
#else
__asm
    LD  HL, 2
    ADD HL, SP
    LD  A, (HL)   ; tval
    SWAPNIB
    AND A, %00001111
    LD  HL, 1
    CP  A, PNT
    JR  Z, ASMPC+3   ; skip the DEC HL
    DEC HL
__endasm
#endif
}

ubyte tile_is_track(ubyte tval)
{
#define tile_is_track_ASM
#ifndef tile_is_track_ASM
    ubyte hi = HINIB(tval);
    ubyte activebits = 0;
    activebits += ((hi & 8) >> 3);
    activebits += ((hi & 4) >> 2);
    activebits += ((hi & 2) >> 1);
    activebits += ((hi & 1) >> 0);
    return activebits == 2;
#else
    // ASM VERSION CLOBBERS A, B, HL
__asm
    LD  HL, 2
    ADD HL, SP
    LD  A, (HL)   ; tval

    SWAPNIB
    LD  B, 0

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    LD  HL, 1
    LD  A, B
    CP  A, 2
    JR  Z, ASMPC+3   ; skip the DEC HL
    DEC HL
__endasm
#endif
}

ubyte tile_is_fixed(ubyte tval)
{
#define tile_is_fixed_ASM
#ifndef tile_is_fixed_ASM
    ubyte hi = HINIB(tval);
    ubyte activebits = 0;
    activebits += ((hi & 8) >> 3);
    activebits += ((hi & 4) >> 2);
    activebits += ((hi & 2) >> 1);
    activebits += ((hi & 1) >> 0);
    return activebits >= 3;
#else
__asm
    LD  HL, 2
    ADD HL, SP
    LD  A, (HL)   ; tval

    SWAPNIB
    LD  B, 0

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    LD  HL, 1
    LD  A, B
    CP  A, 3
    JR  NC, ASMPC+3   ; skip the DEC HL
    DEC HL
__endasm
#endif
}

ubyte tile_has3exits(ubyte tval)
{
#define tile_has3exits_ASM
#ifndef tile_has3exits_ASM
    ubyte hi = HINIB(tval);
    ubyte lo = LONIB(tval);
    ubyte hilo = hi | lo;
    ubyte activebits = 0;
    activebits += ((hilo & 8) >> 3);
    activebits += ((hilo & 4) >> 2);
    activebits += ((hilo & 2) >> 1);
    activebits += ((hilo & 1) >> 0);
    return activebits == 3;
#else
    // ASM VERSION CLOBBERS A, B, HL
__asm
    LD  HL, 2
    ADD HL, SP
    LD  A, (HL)   ; tval

    LD  B, A
    SWAPNIB
    OR  A, B
    AND A, %00001111

    LD  B, 0

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    RRCA
    JR NC, ASMPC+3   ; skip the INC B
    INC B

    LD  HL, 1
    LD  A, B
    CP  A, 3
    JR  Z, ASMPC+3   ; skip the DEC HL
    DEC HL
__endasm
#endif
}



#define FRAME_BOARD_CORNER 76
#define FRAME_BOARD_STRAIGHT 77

#define FRAME_BOARD_X0 BOARD_X - 1
#define FRAME_BOARD_Y0 BOARD_Y - 1
#define FRAME_BOARD_W  2 + BOARD_W * 4
#define FRAME_BOARD_H  2 + BOARD_H * 4

void board_draw_frame()
{
    print_set_symbol(BLOCK_DCUR);
    print_frame(FRAME_BOARD_X0, FRAME_BOARD_Y0, FRAME_BOARD_W, FRAME_BOARD_H);
}




// board_draw_frame
//
// #define X0 BOARD_X - 1
// #define Y0 BOARD_Y - 1
// #define X1 BOARD_X + BOARD_W * 4
// #define Y1 BOARD_Y + BOARD_H * 4

    // tilemap_put_free_tile(X0, Y0, FRAME_BOARD_CORNER, ATTR_____);
    // for (ubyte j = 0; j < BOARD_H * 4; j++)
    //     tilemap_put_free_tile(X0, BOARD_Y + j, FRAME_BOARD_STRAIGHT, ATTR_____);
    // tilemap_put_free_tile(X0, BOARD_Y + BOARD_H * 4, FRAME_BOARD_CORNER, ATTR__Y__);
    // for (ubyte i = 0; i < BOARD_W * 4; i++)
    //     tilemap_put_free_tile(BOARD_X + i, Y0, FRAME_BOARD_STRAIGHT, ATTR___R_);
    // tilemap_put_free_tile(X1, Y0, FRAME_BOARD_CORNER, ATTR_X___);
    // for (ubyte j = 0; j < BOARD_H * 4; j++)
    //     tilemap_put_free_tile(X1, BOARD_Y + j, FRAME_BOARD_STRAIGHT, ATTR_X___);
    // tilemap_put_free_tile(X1, Y1, FRAME_BOARD_CORNER, ATTR_XY__);
    // for (ubyte i = 0; i < BOARD_W * 4; i++)
    //     tilemap_put_free_tile(BOARD_X + i, Y1, FRAME_BOARD_STRAIGHT, ATTR__YR_);


// void tilemap_put_free_tile(ubyte x, ubyte y, ubyte tidx, ubyte attr)
// {
// #define tilemap_put_free_tile_ASM
// #ifndef tilemap_put_free_tile_ASM
//     ubyte* dst = tmstart;
//     dst += 80 * y;
//     dst += 2 * x;
//     dst[0] = tidx;
//     dst[1] = attr;
// #else
// __asm
//     LD  HL, 8
//     ADD HL, SP
//     LD  B, (HL)  ;x
    
//     LD  HL, 6
//     ADD HL, SP
//     LD  C, (HL)  ;y

//     LD  H, 0
//     LD  L, C
//     ADD HL, HL
//     ADD HL, HL
//     ADD HL, HL
//     ADD HL, HL
//     LD  DE, HL
//     ADD HL, HL
//     ADD HL, HL
//     ADD HL, DE   ; 80*y
//     LD  DE, HL

//     LD  H, 0
//     LD  L, B
//     ADD HL, HL   ; 2*x
//     ADD HL, DE   ; 2*x + 80*y
//     LD  DE, HL

//     LD  HL, (_tmstart)
//     ADD HL, DE
//     LD  DE, HL

//     LD  HL, 4
//     ADD HL, SP
//     LD  A, (HL)   ; tidx
//     LD  (DE), A
//     INC DE

//     LD  HL, 2
//     ADD HL, SP
//     LD  A, (HL)   ; attr
//     LD  B, A
//     LD  A, (_curr_print_attr)
//     OR  A, B
//     LD  (DE), A

// __endasm
// #endif
// }

