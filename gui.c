#include "gui.h"
#include "tilesymbols.h"
#include "colors.h"
#include "level.h"
#include "train.h"
#include "board.h"
#include "print_tile.h"

extern ubyte* tmstart;
/*
void clear_screen()
{
    ubyte* dst = tmstart;
    uword cnt = 1280;
    while(cnt--) {
        *dst++ = TILE_VOID;
        *dst++ = 0;
    }
}
void test_screen()
{
    ubyte a = 0;
    ubyte b = 0;

    ubyte* dst = tmstart;
    uword cnt = 1280;
    while(cnt--) {
        if (a >= 80) {
            a = 0;
            b++;
        }
        *dst++ = a++;
        *dst++ = b << 4;
    }

}
*/
void gui_clear_screen()
{
#define gui_clear_screen_ASM
#ifndef gui_clear_screen_ASM
    ubyte* dst = tmstart;
    uword cnt = 1280;
    while(cnt--) {
        *dst++ = TILE_VOID;
        *dst++ = 0;
    }
#else
__asm
    LD  A, 0
    LD  HL, (_tmstart)
    LD  C, 32
    gui_clear_screen_outer:
        LD  B, 40
        gui_clear_screen_inner:
            LD (HL), TILE_VOID
            INC HL
            LD (HL), 0
            INC HL
        DJNZ gui_clear_screen_inner
        DEC C
        CP C
    JR NZ, gui_clear_screen_outer
__endasm
#endif
}

#define GCSP_J0 1
#define GCSP_DJ 30
#define GCSP_J1 31
#define GCSP_I0 30
#define GCSP_DI 10
#define GCSP_I1 40

#define GCSP_N0 GCSP_J0 * 80 + 2 * GCSP_I0
#define GCSP_AN GCSP_DI
#define GCSP_IN GCSP_I0 * 2

void gui_clear_side_panel()
{
#define gui_clear_side_panel_ASM
#ifndef gui_clear_side_panel_ASM
    ubyte* dst;
    for (ubyte j = 1; j < 31; j++) {
        dst = tmstart + (80*j);
        dst += 2 * 29;
        for (ubyte i = 0; i < 11; i++) {
            *dst++ = TILE_VOID;
            *dst++ = 0;
        }
    }
#else
__asm
    LD  A, 0
    LD  HL, (_tmstart)
    LD  DE, GCSP_N0
    ADD HL, DE
    LD  C, GCSP_DJ
    gui_clear_side_panel_outer:
        LD  B, GCSP_AN
        gui_clear_side_panel_inner:
            LD (HL), TILE_VOID
            INC HL
            LD (HL), 0
            INC HL
        djnz gui_clear_side_panel_inner
        LD DE, GCSP_IN
        ADD HL, DE
        DEC C
        CP  C
    JR NZ, gui_clear_side_panel_outer
__endasm
#endif
}


#include "print_tile.h"

#define GDSF_X 32
#define GDSF_Y0 27
#define GDSF_Y1 28

#define GDSJ_X 31
#define GDSJ_Y0 24
#define GDSJ_Y1 25

void gui_display_train_frame()
{
    print_set_pos(GDSF_X, GDSF_Y0);
    print_set_color(CTEXT_WHI);
    print_str("FRAME:");
    print_set_pos(GDSF_X, GDSF_Y1);
    print_char('$');
    print_hex_word(train_abs_frame);
}

void gui_display_train_steps()
{
    print_set_pos(GDSJ_X, GDSJ_Y0);
    print_set_color(CTEXT_WHI);
    print_str("JOURNEY:");
    print_set_pos(GDSJ_X, GDSJ_Y1);
    if (train_steps < 10000) print_char(' ');
    if (train_steps < 1000) print_char(' ');
    if (train_steps < 100) print_char(' ');
    if (train_steps < 10) print_char(' ');
    print_char(' ');
    print_dec_word(train_steps);
}

void guibutton_draw(GuiButton* gb)
{
    print_set_symbol(BLOCK_BUTTON_BORDER);
    print_frame(gb->x, gb->y, gb->w, gb->h);
}

void guibutton_draw_fine(GuiButton* gb)
{
    print_set_symbol(BLOCK_FINE_BORDER);
    print_frame(gb->x, gb->y, gb->w, gb->h);
}

void guibutton_clear(GuiButton* gb)
{
    print_set_symbol(TILE_VOID);
    print_rect_symbol(gb->x, gb->y, gb->w, gb->h);
}

#include "mouse.h"

ubyte guibutton_mouseinto(GuiButton* gb)
{
    ubyte x0 = (gb->x * 4) + 2;
    if (mouse_hx < x0) return 0;

    ubyte y0 = (gb->y * 4) + 2;
    if (mouse_hy < y0) return 0;

    ubyte x1 = x0 + (gb->w * 4) - 4;
    if (mouse_hx > x1) return 0;

    ubyte y1 = y0 + (gb->h * 4) - 4;
    if (mouse_hy > y1) return 0;

    return 1;
}










// #define GUI_FRAME_CORNER 74
// #define GUI_FRAME_STRAIGHT 75

// void gui_draw_frame(ubyte x0, ubyte y0, ubyte w, ubyte h)
// {
//     ubyte x1 = x0 + w - 1;
//     ubyte y1 = y0 + h - 1;
//     tilemap_put_free_tile(x0, y0, GUI_FRAME_CORNER, ATTR_____);
//     tilemap_put_free_tile(x0, y1, GUI_FRAME_CORNER, ATTR__Y__);
//     tilemap_put_free_tile(x1, y0, GUI_FRAME_CORNER, ATTR_X___);
//     tilemap_put_free_tile(x1, y1, GUI_FRAME_CORNER, ATTR_XY__);
    
//     for (ubyte j = y0+1; j < y1; j++) {
//         tilemap_put_free_tile(x0, j, GUI_FRAME_STRAIGHT, ATTR_____);
//         tilemap_put_free_tile(x1, j, GUI_FRAME_STRAIGHT, ATTR_X___);
//     }
//     for (ubyte i = x0+1; i < x1; i++) {
//         tilemap_put_free_tile(i, y0, GUI_FRAME_STRAIGHT, ATTR___R_);
//         tilemap_put_free_tile(i, y1, GUI_FRAME_STRAIGHT, ATTR__YR_);
//     }
// }

