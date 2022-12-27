#ifndef __board_h__
#define __board_h__

#include "types.h"

#define BOARD_SZ 49
#define BOARD_W 7
#define BOARD_H 7
#define BOARD_X 1
#define BOARD_Y 3

#define BOARD_CTR_X(I) (BOARD_X*8 + 16 + 32*I)
#define BOARD_CTR_Y(J) (BOARD_Y*8 + 16 + 32*J)

extern ubyte board[BOARD_SZ];

void board_clear(ubyte* dst);
void board_clear_tracks(ubyte* dst);

void board_draw();

ubyte board_get_tile(ubyte i, ubyte j);
void board_set_tile(ubyte i, ubyte j, ubyte tval);
void board_draw_tile(ubyte i, ubyte j, ubyte tval);

ubyte tile_is_sink(ubyte tval);
ubyte tile_is_paint(ubyte tval);
ubyte tile_is_track(ubyte tval);
ubyte tile_is_fixed(ubyte tval);

ubyte tile_has3exits(ubyte val);

void board_draw_frame();

void board_copy(ubyte* src, ubyte* dst);
void board_copy_tracks(ubyte* src, ubyte *dst);
// void board_copy_not_fixed(ubyte* src, ubyte *dst);

ubyte board_is_equal(ubyte* board1, ubyte* board2);

// tilemap start address
extern ubyte* tmstart;

// draw tiles raw
extern ubyte bdr_tidx;
extern ubyte bdr_x0;
extern ubyte bdr_y0;
extern void __draw_tile_xy();
#define draw_tile_xy(ti, tj, idx) \
{ bdr_tidx = idx; bdr_x0 = ti; bdr_y0 = tj; __draw_tile_xy();}




#endif