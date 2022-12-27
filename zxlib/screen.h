////////////////////////////////////////////////////////////////////////////////
//
// Tetris for ZX Spectrum 48K
//
// (C) 2019, 2020 David Crespo - https://github.com/dcrespo3d/zx-spectrum-tetris
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _screen_h_
#define _screen_h_

#include "params0.h"

uword screen_get_bmap_addr(ubyte col, ubyte row);
uword screen_get_attr_addr(ubyte col, ubyte row);

void screen_print_char_raw(uword addr, ubyte ch);
void screen_print_char(ubyte col, ubyte row, ubyte ch);
void screen_print_attr(ubyte col, ubyte row, ubyte attr);

void screen_print_string(ubyte col, ubyte row, const char* str);

void screen_print_user_raw(uword addr, uword user);
void screen_print_user(ubyte col, ubyte row, uword user);

void screen_clear_bmap(ubyte value);
void screen_clear_attr(ubyte value);

void screen_clear_bmap_blocks(ubyte value, ubyte x, ubyte y, ubyte w, ubyte h);
void screen_clear_attr_blocks(ubyte value, ubyte x, ubyte y, ubyte w, ubyte h);

void screen_border_set(ubyte color);

#endif // _screen_h_
