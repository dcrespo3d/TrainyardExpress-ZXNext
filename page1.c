#include "types.h"
uword p1_dummy_uword_in_zero_address = 0xCAFE;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######
//     #     #    #   #####   ####   #####      #      ##    #
//     #     #    #     #    #    #  #    #     #     #  #   #
//     #     #    #     #    #    #  #    #     #    #    #  #
//     #     #    #     #    #    #  #####      #    ######  #
//     #     #    #     #    #    #  #   #      #    #    #  #
//     #      ####      #     ####   #    #     #    #    #  ######
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "design.h"
#include "gui.h"
#include "colors.h"
#include "level.h"
#include "spmgr.h"
#include "print_tile.h"
#include "screen_controller.h"
#include "board.h"
#include "mouse.h"
#include "screen_levels.h"
#include "screen_game.h"
#include "tilesymbols.h"
#include "player_data.h"

uword tut_frame = 0;
sbyte inc_frame = 1;

ubyte st_use_dec_mode = 0;

void st_update_0();
void st_update_4();
void st_update_10();
void st_update_15();
void st_update_20();
void st_update_27();
void st_update_45();

typedef void (*TutUpdateFunc)();
TutUpdateFunc st_update_func;

#define ST_NB_X 30
#define ST_NB_Y 22
#define ST_NB_W 10
#define ST_NB_H 5

#define ST_SB_X 30
#define ST_SB_Y 27
#define ST_SB_W 10
#define ST_SB_H 5

GuiButton st_btn_next;
GuiButton st_btn_skip;

void st_init_btn_next()
{
    GUIBUTTON_INIT(st_btn_next, ST_NB_X, ST_NB_Y, ST_NB_W, ST_NB_H);
    print_set_color(CTEXT_GRE);
    guibutton_draw(&st_btn_next);
    print_set_color(CTEXT_WHI);
    print_set_pos_inc(1, 2);
    println_ctr("NEXT", 8);
}

void st_init_btn_skip()
{
    GUIBUTTON_INIT(st_btn_skip, ST_SB_X, ST_SB_Y, ST_SB_W, ST_SB_H);
    print_set_color(CTEXT_ORA);
    guibutton_draw(&st_btn_skip);
    print_set_color(CTEXT_WHI);
    print_set_pos_inc(1, 2);
    println_ctr("Skip", 8);
}

void st_hide_btn_next()
{
    GUIBUTTON_INIT(st_btn_next, 40, 0, 2, 2);
    print_set_color(CTEXT_WHI);
    print_set_symbol(DBT_OFFSET + ' ');
    print_rect_symbol(ST_NB_X, ST_NB_Y, ST_NB_W, ST_NB_H);
}

void st_hide_btn_skip()
{
    GUIBUTTON_INIT(st_btn_skip, 40, 0, 2, 2);
    print_set_color(CTEXT_WHI);
    print_set_symbol(DBT_OFFSET + ' ');
    print_rect_symbol(ST_SB_X, ST_SB_Y, ST_SB_W, ST_SB_H);
}

void st_on_next_btn()
{
    if (!st_use_dec_mode) {
        tut_frame += 1;
        inc_frame = 1;
    }
    else {
        tut_frame -= 1;
        inc_frame = -1;
    }
}

void st_on_skip_btn()
{
    sc_switch_screen(sl_enter, sl_update, NULL);
}

#define ST_AB_X 30
#define ST_AB_Y 16
#define ST_AB_W 10
#define ST_AB_H 6

#define ST_PB_X 30
#define ST_PB_Y 22
#define ST_PB_W 10
#define ST_PB_H 6

GuiButton st_btn_again;
GuiButton st_btn_puzzle;

void st_init_btn_again()
{
    GUIBUTTON_INIT(st_btn_again, ST_AB_X, ST_AB_Y, ST_AB_W, ST_AB_H);
    print_set_color(CTEXT_BLU);
    guibutton_draw(&st_btn_again);
    print_set_color(CTEXT_WHI);
    print_set_pos_inc(1, 2);
    println_ctr("WATCH", 8);
    println_ctr("AGAIN", 8);
}

void st_init_btn_puzzle()
{
    GUIBUTTON_INIT(st_btn_puzzle, ST_PB_X, ST_PB_Y, ST_PB_W, ST_PB_H);
    print_set_color(CTEXT_GRE);
    guibutton_draw(&st_btn_puzzle);
    print_set_color(CTEXT_WHI);
    print_set_pos_inc(1, 2);
    println_ctr("NEXT", 8);
    println_ctr("PUZZLE", 8);
}

void st_hide_btn_again()
{
    GUIBUTTON_INIT(st_btn_again, 40, 0, 2, 2);
    print_set_color(CTEXT_WHI);
    print_set_symbol(DBT_OFFSET + ' ');
    print_rect_symbol(ST_AB_X, ST_AB_Y, ST_AB_W, ST_AB_H);
}

void st_hide_btn_puzzle()
{
    GUIBUTTON_INIT(st_btn_puzzle, 40, 0, 2, 2);
    print_set_color(CTEXT_WHI);
    print_set_symbol(DBT_OFFSET + ' ');
    print_rect_symbol(ST_PB_X, ST_PB_Y, ST_PB_W, ST_PB_H);
}

void p1_st_enter();
void st_on_again_btn()
{
    level_stop_trains();
    p1_st_enter();
}

void st_on_puzzle_btn()
{
    level_stop_trains();
    sc_switch_next_level();
}

void st_mark_tutorial_watched()
{
    if (!pd_levels_solved[sg_level_idx]) {
        pd_levels_solved[sg_level_idx] = 1;
        pd_save_levels_solved();
    }
}

// wrappers for saving precious bytes
// by not passing the same parameter again and again
void st_println_ctr_20(const char* txt) {
    println_ctr(txt, 20);
}

uword fake_mx;
ubyte fake_my;
uword fake_target_mx;
ubyte fake_target_my;
ubyte fake_target_delta;

void st_set_fake_cursor(uword x, ubyte y)
{
    fake_mx = x;
    fake_target_mx = x;
    fake_my = y;
    fake_target_my = y;
    fake_target_delta = 0;

    fake_hx = fake_mx >> 1;
    fake_hy = fake_my >> 1;
    spmgr_update_fake_cursor(fake_mx, fake_my);
}

void st_set_fake_target(uword x, ubyte y)
{
    fake_target_mx = x;
    fake_target_my = y;
    fake_target_delta = 1;
}

void st_update_fake_cursor_target()
{
    if (fake_target_mx == fake_mx && fake_target_my == fake_my) return;

    if (fake_mx < fake_target_mx) fake_mx += fake_target_delta;
    if (fake_mx > fake_target_mx) fake_mx -= fake_target_delta;

    if (fake_my < fake_target_my) fake_my += fake_target_delta;
    if (fake_my > fake_target_my) fake_my -= fake_target_delta;

    fake_hx = fake_mx >> 1;
    fake_hy = fake_my >> 1;
    spmgr_update_fake_cursor(fake_mx, fake_my);
}

void st_fake_press_l()
{
    fake_pressed_l = 1;
    fake_but_l = 1;
    design_update_fake();
    fake_pressed_l = 0;
    design_update_fake();
}

void st_fake_release_l()
{
    fake_released_l = 1;
    fake_but_l = 0;
    design_update_fake();
    fake_released_l = 0;
    design_update_fake();
}

void st_fake_dblclick_l()
{
    st_fake_press_l();
    st_fake_release_l();
    st_fake_press_l();
    st_fake_release_l();
    fake_dblclick_l = 1;
    design_update_fake();
    fake_dblclick_l = 0;
    design_update_fake();
}

void st_fake_press_r()
{
    fake_pressed_r = 1;
    fake_but_r = 1;
    design_update_fake();
    fake_pressed_r = 0;
    design_update_fake();
}

void st_fake_release_r()
{
    fake_released_r = 1;
    fake_but_r = 0;
    design_update_fake();
    fake_released_r = 0;
    design_update_fake();
}

void p1_st_enter()
{
    spmgr_init();
    spmgr_setup_cursor();
    spmgr_setup_fake_cursor();
    spmgr_setup_marker();
    spmgr_end_fixed_sprites();
    spmgr_hide_marker();

    st_set_fake_cursor(336, 0);  // offscreen
    
    gui_clear_screen();

    print_set_color(CTEXT_WHI);
    board_draw_frame();

    // design_init();

    level_load(0, sg_level_idx);
    level_set_state(LEV_STATE_DESIGN);
    board_draw();

    print_set_color(CTEXT_BLU);
    print_set_pos(0, 1);
    print_str("TUTORIAL");

    print_set_color(CTEXT_WHI);
    print_set_pos(9, 1);
    print_str(lev_name);

    // print_char(' ');
    // print_dec_byte(sg_level_idx);

    st_update_func = st_on_skip_btn;

    switch(sg_level_idx) {
        case  0: st_update_func = st_update_0; break;
        case  4: st_update_func = st_update_4; break;
        case 10: st_update_func = st_update_10; break;
        case 15: st_update_func = st_update_15; break;
        case 20: st_update_func = st_update_20; break;
        case 27: st_update_func = st_update_27; break;
        case 45: st_update_func = st_update_45; break;
    }

    st_hide_btn_next();
    st_hide_btn_skip();
    st_hide_btn_again();
    st_hide_btn_puzzle();

    fake_hx         = 0;
    fake_hy         = 0;
    fake_pressed_l  = 0;
    fake_but_l      = 0;
    fake_dblclick_l = 0;
    fake_released_l = 0;
    fake_pressed_r  = 0;
    fake_but_r      = 0;
    fake_released_r = 0;

    st_use_dec_mode = 0;
    tut_frame = 0;
    inc_frame = 1;
}

void p1_st_update()
{
// #define TUT_DEBUG_DATA
#ifdef TUT_DEBUG_DATA
    print_set_pos(35,0);
    print_set_color(CTEXT_WHI);
    print_dec_word(tut_frame);
    print_set_pos(35,1);
    print_dec_word(inc_frame);

    print_set_pos(35,2);
    print_dec_word(fake_mx);
    print_char(' '); print_char(' '); 
    print_set_pos(35,3);
    print_dec_byte(fake_my);
    print_char(' '); print_char(' '); 
    print_set_pos(35,4);
    print_dec_byte(fake_target_mx);
    print_char(' '); print_char(' '); 
    print_set_pos(35,5);
    print_dec_byte(fake_target_my);
    print_char(' '); print_char(' '); 
    print_set_pos(35,6);
    print_dec_byte(mouse_x);
    print_char(' '); print_char(' '); 
    print_set_pos(35,7);
    print_dec_byte(mouse_y);
    print_char(' '); print_char(' '); 
#endif

    st_update_func();
    st_update_fake_cursor_target();
    level_update();
    design_update_fake();
    tut_frame += inc_frame;
}

void st_update_common()
{
    mouseAction = nullMouseAction;

    if      (GBMI(&st_btn_next)) mouseAction = st_on_next_btn;
    else if (GBMI(&st_btn_skip)) mouseAction = st_on_skip_btn;
    else if (GBMI(&st_btn_again)) mouseAction = st_on_again_btn;
    else if (GBMI(&st_btn_puzzle)) mouseAction = st_on_puzzle_btn;

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();
    
    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######                           ###
//     #     #    #   #####          #   #
//     #     #    #     #           # #   #
//     #     #    #     #           #  #  #
//     #     #    #     #           #   # #
//     #     #    #     #            #   #
//     #      ####      #             ###
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define DT0 50
#define DT02 50

#define T00 1*DT02
#define T01 T00 + 2*DT02
#define T02 T01 + 2*DT02
#define T03 T02 + 1
#define T04 T03 + 2*DT02
#define T05 T04 + 1
#define T06 T05 + 2*DT02
#define T07 T06 + 1
#define T08 T07 + 2*DT0
#define T09 T08 + 128
#define T0A T09 + 1
#define T0B T0A + 1*DT0
#define T0C T0B + 128

__asm
_st_tut0_lev:
    DEFB $F8, $13, $15
    DEFB $E2, $53, $15
    DEFB $00
__endasm;

extern ubyte* st_tut0_lev[];

ubyte st_tut0_tally0;
ubyte st_tut0_tally1;
ubyte st_tut0_tally2;

void st_update_0()
{
    st_update_common();

    if (tut_frame == T00) {

        st_tut0_tally0 = spmgr_curr_idx;
        spmgr_setup_tally(232, 188, (CTEXT_YEL * 16 | ATTR___R_));
        spmgr_hide_tally(st_tut0_tally0);

        st_tut0_tally1 = spmgr_curr_idx;
        spmgr_setup_tally( 48, 104, (CTEXT_YEL * 16 | ATTR__Y__));
        spmgr_hide_tally(st_tut0_tally1);

        st_tut0_tally2 = spmgr_curr_idx;
        spmgr_setup_tally(176, 152, (CTEXT_YEL * 16 | ATTR_____));
        spmgr_hide_tally(st_tut0_tally2);

        spmgr_end_fixed_sprites();

        print_set_color(CTEXT_WHI);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        println("");
        st_println_ctr_20("Welcome to");
        st_println_ctr_20("Trainyard!");

        return;
    }

    if (tut_frame == T01) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 14, 22, 6);
        st_println_ctr_20("This tutorial will");
        st_println_ctr_20("explain how to");
        st_println_ctr_20("solve puzzles");

        return;
    }

    if (tut_frame == T02) {
        spmgr_show_tally(st_tut0_tally0);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 22, 22, 6);
        st_println_ctr_20("Press NEXT to");
        st_println_ctr_20("advance through");
        st_println_ctr_20("the steps");

        st_init_btn_skip();
        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    if (tut_frame == T03) {
        spmgr_hide_tally(st_tut0_tally0);

        st_hide_btn_next();
        board_draw();

        level_parse_ptr = (ubyte*)st_tut0_lev;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        spmgr_show_tally(st_tut0_tally1);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 5, 22, 5);
        st_println_ctr_20("OUTLET stations");
        st_println_ctr_20("have a PLUS ");
        st_println_ctr_20("symbol on them");
        return;
    }

    if (tut_frame == T04) {
        spmgr_show_tally(st_tut0_tally2);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 24, 22, 5);
        st_println_ctr_20("GOAL stations");
        st_println_ctr_20("have a CIRCLE");
        st_println_ctr_20("on them");

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    if (tut_frame == T05) {
        st_hide_btn_next();
        board_draw();

        spmgr_hide_tally(st_tut0_tally2);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 5, 22, 5);
        st_println_ctr_20("You've gotta get");
        st_println_ctr_20("the GREEN train from");
        st_println_ctr_20("this OUTLET station");

        return;
    }

    if (tut_frame == T06) {
        spmgr_show_tally(st_tut0_tally2);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 24, 22, 5);
        println("");
        st_println_ctr_20("to this GOAL station");

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    if (tut_frame == T07) {
        st_hide_btn_next();
        board_draw();

        spmgr_hide_tally(st_tut0_tally1);
        spmgr_hide_tally(st_tut0_tally2);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(3, 4, 24, 6);
        println_ctr("Your job is", 22);
        println_ctr("to draw track.", 22);
        println_ctr("You'll use the cursor", 22);
        println_ctr("to drag a line", 22);

        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();

        return;
    }

    if (tut_frame == T08) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(3));
        return;
    }

    if (tut_frame == T09) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(3, 24, 24, 6);
        println_ctr("You can draw as much", 22);
        println_ctr("track as you want.", 22);
        println_ctr("There's no limit", 22);
        println_ctr("or score", 22);

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    if (tut_frame == T0A) {
        st_fake_release_l();
        spmgr_hide_marker();
        st_set_fake_cursor(336, 0);  // offscreen
    
        st_hide_btn_next();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(6, 4, 18, 6);
        println("");
        println_ctr("Let's watch", 16);
        println_ctr("the train go...", 16);

        return;
    }

    if (tut_frame == T0B) {
        level_set_speed(0);
        level_start_trains();
        return;
    }

    if (tut_frame == T0C) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(6, 24, 18, 5);
        println("");
        println_ctr("Yay!", 16);

        tut_frame ++;
        inc_frame = 0;

        st_hide_btn_skip();
        st_hide_btn_next();
        st_init_btn_again();
        st_init_btn_puzzle();

        st_mark_tutorial_watched();
        return;
    }
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######                         #
//     #     #    #   #####         #    #
//     #     #    #     #           #    #
//     #     #    #     #           #######
//     #     #    #     #                #
//     #     #    #     #                #
//     #      ####      #                #
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define DT1  50
#define DT12 32

#define T10 25
#define T11 T10 + 2*DT1
#define T12 T11 + DT12
#define T13 T12 + DT12
#define T14 T13 + DT12
#define T15 T14 + 1
#define T16 T15 + 1*DT1
#define T17 T16 + 1
#define T18 T17 + DT12
#define T19 T18 + 64
#define T1A T19 + 32
#define T1B T1A + 32
#define T1C T1B + 64
#define T1D T1C + 32
#define T1E T1D + 32
#define T1F T1E + 64
#define T1G T1F + 256

__asm
_st_tut1_lev:
    DEFB $F4, $51, $14
    DEFB $E8, $15, $14
    DEFB $00
__endasm;

extern ubyte* st_tut1_lev[];

ubyte st_tut1_tally0;

void st_update_4()
{
    if (tut_frame == 0)
        st_init_btn_skip();

    st_update_common();

    if (tut_frame == T10) {
        st_tut1_tally0 = spmgr_curr_idx;
        spmgr_setup_tally(62, 174, (CTEXT_YEL * 16 | ATTR__Y__));
        spmgr_hide_tally(st_tut1_tally0);

        print_set_color(CTEXT_WHI);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 4, 22, 6);
        st_println_ctr_20("To draw a corner,");
        st_println_ctr_20("drag the cursor from");
        st_println_ctr_20("one side and exit");
        st_println_ctr_20("from another side");

        return;
    }

    if (tut_frame == T11)
    {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(2));
        st_fake_press_l();

        return;
    }

    if (tut_frame == T12)
    {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    if (tut_frame == T13)
    {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(3));
        return;
    }

    if (tut_frame == T14)
    {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 25, 22, 5);
        println("");
        st_println_ctr_20("Ta da!");

        st_fake_release_l();
        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    if (tut_frame == T15)
    {
        st_hide_btn_next();

        st_set_fake_cursor(336, 0);  // offscreen

        board_clear(board);
        level_parse_ptr = (ubyte*)st_tut1_lev;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(2, 6, 18, 6);
        println("");
        println_ctr("Let's look at", 16);
        println_ctr("an actual puzzle", 16);

        return;
    }

    if (tut_frame == T16)
    {
        spmgr_show_tally(st_tut1_tally0);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(2, 14, 18, 6);
        println_ctr("The WHITE side", 16);
        println_ctr("of a GOAL is", 16);
        println_ctr("the IN side", 16);

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    if (tut_frame == T17)
    {
        spmgr_hide_tally(st_tut1_tally0);

        board_draw();
        st_set_fake_cursor(BOARD_CTR_X(5), BOARD_CTR_Y(1));
        st_fake_press_l();
        return;
    }

    if (tut_frame == T18) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(3));
        return;
    }
    if (tut_frame == T19) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(3));
        return;
    }
    if (tut_frame == T1A) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(4));
        return;
    }
    if (tut_frame == T1B) {
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(4));
        return;
    }
    if (tut_frame == T1C) {
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(5));
        return;
    }
    if (tut_frame == T1D) {
        st_set_fake_target(BOARD_CTR_X(1), BOARD_CTR_Y(5));
        return;
    }
    if (tut_frame == T1E) {
        st_set_fake_cursor(336, 0);  // offscreen
        st_fake_release_l();
        return;
    }

    if (tut_frame == T1F)
    {
        level_set_speed(0);
        level_start_trains();
        return;
    }

    if (tut_frame == T1G)
    {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(6, 9, 10, 5);
        println("");
        println_ctr(" Nice!", 8);

        tut_frame ++;
        inc_frame = 0;

        st_hide_btn_skip();
        st_hide_btn_next();
        st_init_btn_again();
        st_init_btn_puzzle();

        st_mark_tutorial_watched();
        return;
    }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######                            #      ###
//     #     #    #   #####           ##     #   #
//     #     #    #     #            # #    # #   #
//     #     #    #     #              #    #  #  #
//     #     #    #     #              #    #   # #
//     #     #    #     #              #     #   #
//     #      ####      #            #####    ###
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

__asm
_st_tut2_lev:
    DEFB $F8, $05, $12
    DEFB $E2, $65, $12
    DEFB $00
__endasm;

extern ubyte* st_tut2_lev[];

void st_update_10()
{
    st_update_common();

    if (tut_frame == 0)
        st_init_btn_skip();

    #define T20 0
    #define DT2 32

    #define T21 T20 + DT2
    if (tut_frame == T21) {
        level_parse_ptr = (ubyte*)st_tut2_lev;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("Sometimes when you");
        st_println_ctr_20("draw you'll make");
        st_println_ctr_20("a few mistakes");

        return;
    }

    #define T22 T21 + DT2
    if (tut_frame == T22) {
        st_set_fake_cursor(BOARD_CTR_X(0), BOARD_CTR_Y(5));
        st_fake_press_l();
        return;
    }

    #define T23 T22 + DT2
    if (tut_frame == T23) {
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(5));
        return;
    }

    #define T24 T23 + DT2 * 2
    if (tut_frame == T24) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(3));
        return;
    }

    #define T25 T24 + DT2 * 2
    if (tut_frame == T25) {
        st_set_fake_cursor(BOARD_CTR_X(4), BOARD_CTR_Y(6)+10);
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(4)+10);
        return;
    }

    #define T26 T25 + DT2 * 2
    if (tut_frame == T26) {
        st_set_fake_cursor(BOARD_CTR_X(2), BOARD_CTR_Y(6)+10);
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5)+10);
    }

    #define T27 T26 + DT2
    if (tut_frame == T27) {
        st_set_fake_target(BOARD_CTR_X(6), BOARD_CTR_Y(5)+10);
        return;
    }

    #define T28 T27 + DT2 * 3
    if (tut_frame == T28) {
        st_fake_release_l();
        st_set_fake_cursor(336, 0);

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T29 T28 + 1
    if (tut_frame == T29) {
        st_hide_btn_next();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("What a mess!");
        st_println_ctr_20("Let's clean it");
        st_println_ctr_20("up by drawing");
        st_println_ctr_20("over it again");

        return;
    }

    #define T2A T29 + DT2
    if (tut_frame == T2A) {
        st_set_fake_cursor(BOARD_CTR_X(0), BOARD_CTR_Y(5));
        st_fake_press_l();
        return;
    }

    #define T2B T2A + DT2
    if (tut_frame == T2B) {
        st_set_fake_target(BOARD_CTR_X(6), BOARD_CTR_Y(5));
        return;
    }

    #define T2C T2B + DT2 * 6
    if (tut_frame == T2C) {
        st_fake_release_l();
        st_set_fake_cursor(336, 0);

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T2D T2C + 1
    if (tut_frame == T2D) {
        st_hide_btn_next();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("If we draw over it");
        st_println_ctr_20("one more time,");
        st_println_ctr_20("it'll be perfect");

        return;
    }

    #define T2E T2D + DT2
    if (tut_frame == T2E) {
        st_set_fake_cursor(BOARD_CTR_X(0), BOARD_CTR_Y(5));
        st_fake_press_l();
    }

    #define T2F T2E + DT2
    if (tut_frame == T2F) {
        st_set_fake_target(BOARD_CTR_X(6), BOARD_CTR_Y(5));
        return;
    }

    #define T2G T2F + DT2 * 6
    if (tut_frame == T2G) {
        st_fake_release_l();
        st_set_fake_cursor(336, 0);

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T2H T2G + 1
    if (tut_frame == T2H) {
        st_hide_btn_next();

        board_clear(board);
        level_init();
        spmgr_clear_level();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 4, 22, 6);
        println("");
        st_println_ctr_20("Let's look at why");
        st_println_ctr_20("this technique works");

        return;
    }

    #define T2I T2H + DT2 * 2
    if (tut_frame == T2I) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 12, 22, 6);
        st_println_ctr_20("Every tile can hold");
        st_println_ctr_20("only TWO pieces of");
        st_println_ctr_20("track at a time");

        return;
    }

    #define T2J T2I + DT2 * 2
    if (tut_frame == T2J) {
        st_set_fake_cursor(BOARD_CTR_X(2), BOARD_CTR_Y(5));
        st_fake_press_l();
        return;
    }

    #define T2K T2J + DT2
    if (tut_frame == T2K) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5));
        return;
    }

    #define T2L T2K + DT2
    if (tut_frame == T2L) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(4));
        return;
    }

    #define T2M T2L + DT2 * 2
    if (tut_frame == T2M) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5));
        return;
    }

    #define T2N T2M + DT2
    if (tut_frame == T2N) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(5));
        return;
    }

    #define T2O T2N + DT2 * 2
    if (tut_frame == T2O) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5));
        return;
    }

    #define T2P T2O + DT2
    if (tut_frame == T2P) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(6));
        return;
    }

    #define T2Q T2P + DT2 * 2
    if (tut_frame == T2Q) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5));
        return;
    }

    #define T2R T2Q + DT2
    if (tut_frame == T2R) {
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(5));
        return;
    }

    #define T2S T2R + DT2 * 2
    if (tut_frame == T2S) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(5));
        return;
    }

    #define T2T T2S + DT2 * 3
    if (tut_frame == T2T) {
        st_fake_release_l();
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(4));
        st_fake_press_l();
        return;
    }

    #define T2U T2T + DT2
    if (tut_frame == T2U) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(6));
        return;
    }

    #define T2V T2U + DT2 * 2
    if (tut_frame == T2V) {
        st_fake_release_l();
        st_set_fake_cursor(336, 0);

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T2W T2V + 1
    if (tut_frame == T2W) {
        st_hide_btn_next();

        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 4, 22, 6);
        st_println_ctr_20("If we draw");
        st_println_ctr_20("the same way");
        st_println_ctr_20("twice in a row...");

        return;
    }

    #define T2X T2W + DT2
    if (tut_frame == T2X) {
        st_set_fake_cursor(BOARD_CTR_X(2), BOARD_CTR_Y(5));
        st_fake_press_l();

        return;
    }

    #define T2Y T2X + DT2
    if (tut_frame == T2Y) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(5));
        return;
    }

    #define T2Z T2Y + DT2 * 3
    if (tut_frame == T2Z) {
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(5));
        return;
    }

    #define T2a T2Z + DT2 * 2
    if (tut_frame == T2a) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 12, 22, 6);
        st_println_ctr_20("...it'll become");
        st_println_ctr_20("a single track");
        st_println_ctr_20("piece again");
        return;
    }

    #define T2b T2a + DT2
    if (tut_frame == T2b) {
        st_fake_release_l();
        st_set_fake_cursor(336, 0);

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T2c T2b + 1
    if (tut_frame == T2c) {
        st_hide_btn_next();

        board_clear(board);
        board[23] = SESE;
        board[24] = WSWS;
        board[30] = NENE;
        board[31] = WENS;
        board[32] = WSWS;
        board[38] = NENE;
        board[39] = NWNW;
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("You can ERASE track");
        st_println_ctr_20("using the RIGHT /");
        st_println_ctr_20("SECONDARY button");

        return;
    }

    #define T2d T2c + DT2
    if (tut_frame == T2d) {
        st_set_fake_cursor(BOARD_CTR_X(2), BOARD_CTR_Y(4));
        return;
    }

    #define T2e T2d + DT2
    if (tut_frame == T2e) {
        st_fake_press_r();
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(4));
        return;
    }

    #define T2f T2e + DT2 * 2
    if (tut_frame == T2f) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(5));
        return;
    }

    #define T2g T2f + DT2
    if (tut_frame == T2g) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5));
        return;
    }

    #define T2h T2g + DT2
    if (tut_frame == T2h) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T2i T2h + DT2 * 2
    if (tut_frame == T2i) {
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(3));
        return;
    }

    #define T2j T2i + DT2 * 2
    if (tut_frame == T2j) {
        st_fake_release_r();
        st_set_fake_cursor(336, 0);

        return;
    }

    #define T2k T2j + DT2
    if (tut_frame == T2k) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 14, 22, 6);
        st_println_ctr_20("Don't forget about");
        st_println_ctr_20("UNDO. You can even");
        st_println_ctr_20("DELETE all tracks!");

        return;
    }

     #define T2l T2k + DT2 * 3
    if (tut_frame == T2l) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 22, 22, 6);
        st_println_ctr_20("Now you've got");
        st_println_ctr_20("lots of ways");
        st_println_ctr_20("to fix mistakes");

        tut_frame ++;
        inc_frame = 0;

        st_hide_btn_skip();
        st_hide_btn_next();
        st_init_btn_again();
        st_init_btn_puzzle();

        st_mark_tutorial_watched();
        return;
    }
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######                            #    #######
//     #     #    #   #####           ##    #
//     #     #    #     #            # #    #
//     #     #    #     #              #     #####
//     #     #    #     #              #          #
//     #     #    #     #              #    #     #
//     #      ####      #            #####   #####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

__asm
_st_tut3_lev:
    DEFB $F8, $13, $22, $20
    DEFB $E2, $63, $12
    DEFB $E2, $64, $12
    DEFB $00
__endasm;

__asm
_st_tut3_lev2:
    DEFB $F8, $13, $24, $20
    DEFB $E2, $63, $12
    DEFB $E2, $64, $14
    DEFB $00
__endasm;

extern ubyte* st_tut3_lev[];
extern ubyte* st_tut3_lev2[];

ubyte st_tut3_tally0;
ubyte st_tut3_tally1;

void st_update_15()
{
    st_update_common();

    if (tut_frame == 0)
        st_init_btn_skip();

    #define DT32 32
    #define T30 25
    if (tut_frame == T30) {
        st_tut3_tally0 = spmgr_curr_idx;
        spmgr_setup_tally(48, 106, (CTEXT_YEL * 16 | ATTR__Y__));
        spmgr_hide_tally(st_tut3_tally0);

        st_tut3_tally1 = spmgr_curr_idx;
        spmgr_setup_tally(112, 110, (CTEXT_YEL * 16 | ATTR__Y__));
        spmgr_hide_tally(st_tut3_tally1);

        spmgr_end_fixed_sprites();

        print_set_color(CTEXT_WHI);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 10, 22, 6);
        println("");
        st_println_ctr_20("Let's learn about");
        st_println_ctr_20("SWITCHING TRACK");

        return;
    }

    #define T31 T30 + DT32 * 2
    if (tut_frame == T31) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 18, 22, 6);
        st_println_ctr_20("SWITCHING TRACK");
        st_println_ctr_20("is one of the");
        st_println_ctr_20("key concepts");
        st_println_ctr_20("in Trainyard");

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T32 T31 + 1
    if (tut_frame == T32) {
        st_hide_btn_next();

        level_parse_ptr = (ubyte*)st_tut3_lev;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(2, 6, 22, 6);
        st_println_ctr_20("This is an");
        st_println_ctr_20("OUTLET station");
        st_println_ctr_20("with TWO RED");
        st_println_ctr_20("trains in it");

        spmgr_show_tally(st_tut3_tally0);

        return;
    }

    #define T33 T32 + DT32
    if (tut_frame == T33) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    #define T34 T33 + DT32
    if (tut_frame == T34) {
        st_set_fake_target(BOARD_CTR_X(6), BOARD_CTR_Y(3));
        return;
    }

    #define T35 T34 + DT32 * 5
    if (tut_frame == T35) {
        st_fake_release_l();
        return;
    }

    #define T36 T35 + DT32
    if (tut_frame == T36) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    #define T37 T36 + DT32
    if (tut_frame == T37) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T38 T37 + DT32 * 2
    if (tut_frame == T38) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(4));
        return;
    }

    #define T39 T38 + DT32
    if (tut_frame == T39) {
        st_set_fake_target(BOARD_CTR_X(6), BOARD_CTR_Y(4));
        return;
    }

    #define T3A T39 + DT32 * 3
    if (tut_frame == T3A) {
        st_fake_release_l();
        return;
    }

    #define T3B T3A + DT32
    if (tut_frame == T3B) {
        st_set_fake_cursor(336, 0);  // offscreen
        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define T3C T3B + 1
    if (tut_frame == T3C) {
        st_hide_btn_next();
        spmgr_hide_tally(st_tut3_tally0);

        level_set_speed(0);
        level_start_trains();
        return;
    }

    #define T3C2 T3C + DT32 * 7
    if (tut_frame == T3C2) {
        board_draw();
        return;
    }

    #define T3D T3C2 + 1
    if (tut_frame == T3D) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        println("");
        st_println_ctr_20("Cool!");

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define T3E T3D + 1
    if (tut_frame == T3E) {
        st_hide_btn_next();

        level_stop_trains();
        
        level_init();
        spmgr_clear_level();
        level_parse_ptr = (ubyte*)st_tut3_lev;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("The ACTIVE track");
        st_println_ctr_20("is brighter and");
        st_println_ctr_20("it's layered above");
        st_println_ctr_20("the PASSIVE track");

        return;
    }

    #define T3F T3E + DT32 * 2
    if (tut_frame == T3F) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 24, 22, 5);
        st_println_ctr_20("Trains will always");
        st_println_ctr_20("take the ACTIVE");
        st_println_ctr_20("track if they can");

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define T3G T3F + 1
    if (tut_frame == T3G) {
        st_hide_btn_next();

        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("You can switch");
        st_println_ctr_20("the track from");
        st_println_ctr_20("PASSIVE to ACTIVE");
        st_println_ctr_20("by drawing over it");

        return;
    }

    #define T3H T3G + DT32
    if (tut_frame == T3H) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    #define T3I T3H + DT32
    if (tut_frame == T3I) {
        st_set_fake_target(BOARD_CTR_X(6), BOARD_CTR_Y(3));
        return;
    }

    #define T3J T3I + DT32 * 6
    if (tut_frame == T3J) {
        st_fake_release_l();
        return;
    }

    #define T3K T3J + DT32
    if (tut_frame == T3K) {
        st_set_fake_cursor(336, 0);  // offscreen
        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define T3L T3K + 1
    if (tut_frame == T3L) {
        st_hide_btn_next();

        level_stop_trains();
        
        level_init();
        spmgr_clear_level();
        level_parse_ptr = (ubyte*)st_tut3_lev;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("You can also switch");
        st_println_ctr_20("tracks with a simple");
        st_println_ctr_20("DOUBLE CLICK or");
        st_println_ctr_20("DOUBLE PRESS");
    }

    #define T3M T3L + DT32
    if (tut_frame == T3M) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T3N T3M + DT32
    if (tut_frame == T3N) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3)+4);
        return;
    }

    #define T3O T3N + 7
    if (tut_frame == T3O) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T3P T3O + 7
    if (tut_frame == T3P) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3)+4);
        return;
    }

    #define T3Q T3P + 7
    if (tut_frame == T3Q) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        st_fake_dblclick_l();
        return;
    }

    #define T3R T3Q + DT32
    if (tut_frame == T3R) {
        st_set_fake_cursor(336, 0);

        level_set_speed(0);
        level_start_trains();
        return;
    }

    #define T3S T3R + DT32 * 7
    if (tut_frame == T3S) {
        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define T3T T3S + 1
    if (tut_frame == T3T) {
        st_hide_btn_next();

        level_stop_trains();

        board_clear(board);        
        level_init();
        spmgr_clear_level();
        level_parse_ptr = (ubyte*)st_tut3_lev2;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("The most important");
        st_println_ctr_20("part: trains switch");
        st_println_ctr_20("the track when");
        st_println_ctr_20("they go over it");
        return;
    }

    #define T3U T3T + DT32
    if (tut_frame == T3U) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    #define T3V T3U + DT32
    if (tut_frame == T3V) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T3W T3V + DT32 * 2
    if (tut_frame == T3W) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(4));
        return;
    }

    #define T3X T3W + DT32
    if (tut_frame == T3X) {
        st_set_fake_target(BOARD_CTR_X(6), BOARD_CTR_Y(4));
        return;
    }

    #define T3Y T3X + DT32 * 3
    if (tut_frame == T3Y) {
        st_fake_release_l();
        return;
    }

    #define T3Z T3Y + DT32
    if (tut_frame == T3Z) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    #define T3a T3Z + DT32
    if (tut_frame == T3a) {
        st_set_fake_target(BOARD_CTR_X(6), BOARD_CTR_Y(3));
        return;
    }

    #define T3b T3a + DT32 * 5
    if (tut_frame == T3b) {
        st_fake_release_l();
        return;
    }

    #define T3c T3b + DT32
    if (tut_frame == T3c) {
        st_set_fake_cursor(336, 0);  // offscreen
        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define T3d T3c + 1
    if (tut_frame == T3d) {
        st_hide_btn_next();

        board_draw();
        level_set_speed(0);
        level_start_trains();

        return;
    }

    #define T3e T3d + DT32 * 8
    if (tut_frame == T3e) {
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 8, 22, 5);
        println("");
        st_println_ctr_20("Uh oh!");

        return;
    }

    #define T3f T3e + DT32 * 3
    if (tut_frame == T3f) {
        level_stop_trains();

        level_init();
        spmgr_clear_level();
        level_parse_ptr = (ubyte*)st_tut3_lev2;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        return;
    }

    #define T3g T3f + DT32
    if (tut_frame == T3g) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T3h T3g + DT32
    if (tut_frame == T3h) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3)+4);
        return;
    }

    #define T3i T3h + 7
    if (tut_frame == T3i) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T3j T3i + 7
    if (tut_frame == T3j) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3)+4);
        return;
    }

    #define T3k T3j + 7
    if (tut_frame == T3k) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        st_fake_dblclick_l();
        return;
    }

    #define T3l T3k + DT32
    if (tut_frame == T3l) {
        st_set_fake_cursor(336, 0);

        level_set_speed(0);
        level_start_trains();
        return;
    }

    #define T3m T3l + DT32 * 7
    if (tut_frame == T3m) {
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        println("");
        st_println_ctr_20("Ah ha!");
        st_println_ctr_20("That's better!");

        tut_frame ++;
        inc_frame = 0;

        st_hide_btn_skip();
        st_hide_btn_next();
        st_init_btn_again();
        st_init_btn_puzzle();

        st_mark_tutorial_watched();
        return;
    }
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######                          #####    ###
//     #     #    #   #####         #     #  #   #
//     #     #    #     #                 # # #   #
//     #     #    #     #            #####  #  #  #
//     #     #    #     #           #       #   # #
//     #     #    #     #           #        #   #
//     #      ####      #           #######   ###
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define DT4  50
#define DT42 32

#define T40 25
#define T41 T40 + 2*DT4
#define T42 T41 + 1
#define T43 T42 + DT42
#define T44 T43 + DT42
#define T45 T44 + 4*DT42
#define T46 T45 + DT42
#define T47 T46 + DT42
#define T48 T47 + 3*DT42
#define T49 T48 + DT42
#define T4A T49 + 2*DT42
#define T4B T4A + DT42
#define T4C T4B + 1
#define T4D T4C + 7*DT42
#define T4E T4D + 3*DT42
#define T4F T4E + 1
#define T4G T4F + 64
#define T4H T4G + 32
#define T4I T4H + 64
#define T4J T4I + 32
#define T4K T4J + 32
#define T4L T4K + 32
#define T4M T4L + 32
#define T4N T4M + 32
#define T4O T4N + 32
#define T4P T4O + 128
#define T4Q T4P + 1
#define T4R T4Q + 224
#define T4S T4R + 1
#define T4T T4S + 1
#define T4U T4T + 1

__asm
_st_tut4_lev:
    DEFB $F8, $04, $14
    DEFB $F8, $13, $14
    DEFB $E2, $53, $14
    DEFB $00
__endasm;

extern ubyte* st_tut4_lev[];

ubyte st_tut4_tally0;
ubyte st_tut4_tally1;
ubyte st_tut4_tally2;

void st_update_20()
{
    if (tut_frame == 0)
        st_init_btn_skip();

    st_update_common();

    if (tut_frame == T40) {
        st_tut4_tally0 = spmgr_curr_idx;
        spmgr_setup_tally(16, 138, (CTEXT_YEL * 16 | ATTR__Y__));
        // spmgr_hide_tally(st_tut4_tally0);

        st_tut4_tally1 = spmgr_curr_idx;
        spmgr_setup_tally(48, 106, (CTEXT_YEL * 16 | ATTR__Y__));
        // spmgr_hide_tally(st_tut4_tally1);

        st_tut4_tally2 = spmgr_curr_idx;
        spmgr_setup_tally(176, 150, (CTEXT_YEL * 16 | ATTR_____));
        spmgr_hide_tally(st_tut4_tally2);

        spmgr_end_fixed_sprites();

        level_parse_ptr = (ubyte*)st_tut4_lev;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_color(CTEXT_WHI);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(2, 6, 22, 6);
        st_println_ctr_20("Notice that there");
        st_println_ctr_20("are TWO yellow");
        st_println_ctr_20("trains starting");
        st_println_ctr_20("on this side...");

        return;
    }

    if (tut_frame == T41) {
        spmgr_show_tally(st_tut4_tally2);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(10, 22, 18, 6);
        println_ctr("but this", 16);
        println_ctr("GOAL station", 16);
        println_ctr("only wants", 16);
        println_ctr("ONE train", 16);

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    if (tut_frame == T42) {
        st_hide_btn_next();

        spmgr_hide_tally(st_tut4_tally0);
        spmgr_hide_tally(st_tut4_tally1);
        spmgr_hide_tally(st_tut4_tally2);

        board_clear_tracks(board);
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("Let's see what");
        st_println_ctr_20("happens if we");
        st_println_ctr_20("solve it like");
        st_println_ctr_20("we usually would");

        return;
    }

    if (tut_frame == T43) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    if (tut_frame == T44) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(3));
        return;
    }

    if (tut_frame == T45) {
        st_fake_release_l();
        return;
    }

    if (tut_frame == T46) {
        st_set_fake_cursor(BOARD_CTR_X(0), BOARD_CTR_Y(4));
        st_fake_press_l();
        return;
    }

    if (tut_frame == T47) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(4));
        return;
    }

    if (tut_frame == T48) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    if (tut_frame == T49) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(3));
        return;
    }

    if (tut_frame == T4A) {
        st_fake_release_l();
        return;
    }

    if (tut_frame == T4B) {
        st_set_fake_cursor(336, 0);  // offscreen
    
        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    if (tut_frame == T4C) {
        st_hide_btn_next();

        board_draw();

        level_set_speed(0);
        level_start_trains();
        return;
    }

    if (tut_frame == T4D) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("That was almost");
        st_println_ctr_20("good, but we");
        st_println_ctr_20("WASTED the second");
        st_println_ctr_20("yellow train");

        return;
    }

    if (tut_frame == T4E) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 24, 22, 6);
        st_println_ctr_20("In Trainyard, you");
        st_println_ctr_20("must use ALL of the");
        st_println_ctr_20("trains and not let");
        st_println_ctr_20("any of them CRASH");

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    if (tut_frame == T4F) {
        st_hide_btn_next();

        level_stop_trains();
        
        board_clear(board);
        level_init();
        spmgr_clear_level();
        level_parse_ptr = (ubyte*)st_tut4_lev;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();


        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("This time, we'll");
        st_println_ctr_20("time the trains so");
        st_println_ctr_20("that they MERGE");
        st_println_ctr_20("into ONE train");

        return;
    }

    if (tut_frame == T4G) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    if (tut_frame == T4H) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    if (tut_frame == T4I) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(4));
        return;
    }

    if (tut_frame == T4J) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(4));
        return;
    }

    if (tut_frame == T4K) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(3));
        return;
    }

    if (tut_frame == T4L) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(3));
        return;
    }

    if (tut_frame == T4M) {
        st_fake_release_l();
        return;
    }

    if (tut_frame == T4N) {
        st_set_fake_cursor(BOARD_CTR_X(0), BOARD_CTR_Y(4));
        st_fake_press_l();
        return;
    }

    if (tut_frame == T4O) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(4));
        return;
    }

    if (tut_frame == T4P) {
        st_init_btn_next();
        inc_frame = 0;

        st_fake_release_l();
        st_set_fake_cursor(336, 0);  // offscreen
        return;
    }

    if (tut_frame == T4Q) {
        st_hide_btn_next();

        level_set_speed(0);
        level_start_trains();
        return;
    }

    if (tut_frame == T4R) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        println("");
        st_println_ctr_20("Hurray!");

        tut_frame ++;
        inc_frame = 0;

        st_hide_btn_skip();
        st_hide_btn_next();
        st_init_btn_again();
        st_init_btn_puzzle();

        st_mark_tutorial_watched();
        return;
    }

}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######                          #####  #######
//     #     #    #   #####         #     # #    #
//     #     #    #     #                 #     #
//     #     #    #     #            #####     #
//     #     #    #     #           #         #
//     #     #    #     #           #         #
//     #      ####      #           #######   #
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

__asm
_st_tut5_lev:
    DEFB $F8, $13, $12
    DEFB $F8, $15, $14
    DEFB $E2, $54, $16
    DEFB $00
__endasm;

__asm
_st_tut5_lev2:
    DEFB $F8, $13, $12
    DEFB $F8, $15, $14
    DEFB $E2, $53, $16
    DEFB $E2, $55, $16
    DEFB $00
__endasm;

__asm
_st_tut5_lev3:
    DEFB $F8, $03, $14
    DEFB $F4, $60, $14
    DEFB $F1, $66, $11
    DEFB $00
__endasm;

extern ubyte* st_tut5_lev[];
extern ubyte* st_tut5_lev2[];
extern ubyte* st_tut5_lev3[];

void st_update_27()
{
    st_update_common();

    if (tut_frame == 0)
        st_init_btn_skip();

    #define DT5 32
    #define T50 0

    #define T51 T50 + DT5
    if (tut_frame == T51) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(2, 10, 26, 6);
        println_ctr("By now, you've", 24);
        println_ctr("discovered that you can", 24);
        println_ctr("MIX colours by MERGING", 24);
        println_ctr("trains together", 24);

        return;
    }

    #define T52 T51 + DT5 * 2
    if (tut_frame == T52) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 18, 22, 6);
        st_println_ctr_20("In this tutorial,");
        st_println_ctr_20("you'll learn two");
        st_println_ctr_20("more MIXING");
        st_println_ctr_20("techniques");

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T53 T52 + 1
    if (tut_frame == T53) {
        st_hide_btn_next();

        level_parse_ptr = (ubyte*)st_tut5_lev;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("Let's start with");
        st_println_ctr_20("MERGING, which");
        st_println_ctr_20("you're probably");
        st_println_ctr_20("used to by now");

        return;
    }

    #define T54 T53 + DT5
    if (tut_frame == T54) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    #define T55 T54 + DT5
    if (tut_frame == T55) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T56 T55 + DT5 * 2
    if (tut_frame == T56) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(4));
        return;
    }

    #define T57 T56 + DT5
    if (tut_frame == T57) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(4));
        return;
    }

    #define T58 T57 + DT5 * 2
    if (tut_frame == T58) {
        st_fake_release_l();
        return;
    }

    #define T59 T58 + DT5
    if (tut_frame == T59) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(5));
        st_fake_press_l();
        return;
    }

    #define T5A T59 + DT5
    if (tut_frame == T5A) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5));
        return;
    }

    #define T5B T5A + DT5 * 2
    if (tut_frame == T5B) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(4));
        return;
    }

    #define T5C T5B + DT5
    if (tut_frame == T5C) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(4));
        return;
    }

    #define T5D T5C + DT5 * 2
    if (tut_frame == T5D) {
        st_fake_release_l();
        st_set_fake_cursor(336, 0);

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T5E T5D + 1
    if (tut_frame == T5E) {
        st_hide_btn_next();

        level_set_speed(0);
        level_start_trains();
        return;
    }

    #define T5F T5E + DT5 * 6
    if (tut_frame == T5F) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("Notice that");
        st_println_ctr_20("MERGING goes from");
        st_println_ctr_20("TWO trains");
        st_println_ctr_20("to ONE train");

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define T5G T5F + 1
    if (tut_frame == T5G) {
        st_hide_btn_next();

        level_stop_trains();
        
        board_clear(board);
        level_init();
        spmgr_clear_level();
        level_parse_ptr = (ubyte*)st_tut5_lev2;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        println("");
        st_println_ctr_20("The next technique");
        st_println_ctr_20("is the CROSSOVER");
        return;
    }

    #define T5H T5G + DT5
    if (tut_frame == T5H) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    #define T5I T5H + DT5
    if (tut_frame == T5I) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T5J T5I + DT5 * 2
    if (tut_frame == T5J) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5));
        return;
    }

    #define T5K T5J + DT5 * 2
    if (tut_frame == T5K) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(5));
        return;
    }

    #define T5L T5K + DT5 * 2
    if (tut_frame == T5L) {
        st_fake_release_l();
        return;
    }

    #define T5M T5L + DT5
    if (tut_frame == T5M) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(5));
        st_fake_press_l();
        return;
    }

    #define T5N T5M + DT5
    if (tut_frame == T5N) {
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(5));
        return;
    }

    #define T5O T5N + DT5
    if (tut_frame == T5O) {
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(4));
        return;
    }

    #define T5P T5O + DT5
    if (tut_frame == T5P) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(4));
        return;
    }

    #define T5Q T5P + DT5 * 2
    if (tut_frame == T5Q) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(3));
        return;
    }

    #define T5R T5Q + DT5
    if (tut_frame == T5R) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(3));
        return;
    }

    #define T5S T5R + DT5
    if (tut_frame == T5S) {
        st_fake_release_l();
        st_set_fake_cursor(336, 0);

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T5T T5S + 1
    if (tut_frame == T5T) {
        st_hide_btn_next();

        level_set_speed(0);
        level_start_trains();
        return;
    }

    #define T5U T5T + DT5 * 7
    if (tut_frame == T5U) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("Notice that a");
        st_println_ctr_20("CROSSOVER goes from");
        st_println_ctr_20("TWO trains to");
        st_println_ctr_20("TWO mixed trains");

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define T5V T5U + 1
    if (tut_frame == T5V) {
        st_hide_btn_next();
        
        level_stop_trains();
        
        board_clear(board);
        level_init();
        spmgr_clear_level();
        level_parse_ptr = (ubyte*)st_tut5_lev2;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("The last mixing");
        st_println_ctr_20("technique is the");
        st_println_ctr_20("COLLISION");
        return;
    }

    #define T5W T5V + DT5
    if (tut_frame == T5W) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    #define T5X T5W + DT5
    if (tut_frame == T5X) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T5Y T5X + DT5 * 2
    if (tut_frame == T5Y) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5));
        return;
    }

    #define T5Z T5Y + DT5 * 2
    if (tut_frame == T5Z) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(5));
        return;
    }

    #define T5a T5Z + DT5 * 2
    if (tut_frame == T5a) {
        st_fake_release_l();
        return;
    }

    #define T5b T5a + DT5
    if (tut_frame == T5b) {
        st_set_fake_cursor(BOARD_CTR_X(1), BOARD_CTR_Y(5));
        st_fake_press_l();
        return;
    }

    #define T5c T5b + DT5
    if (tut_frame == T5c) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5));
        return;
    }

    #define T5d T5c + DT5 * 2
    if (tut_frame == T5d) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T5e T5d + DT5 * 2
    if (tut_frame == T5e) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(3));
        return;
    }

    #define T5f T5e + DT5 * 2
    if (tut_frame == T5f) {
        st_fake_release_l();
        st_set_fake_cursor(336, 0);

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T5g T5f + 1
    if (tut_frame == T5g) {
        st_hide_btn_next();

        level_set_speed(0);
        level_start_trains();

        return;
    }

    #define T5h T5g + DT5 * 5
    if (tut_frame == T5h) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        println("");
        st_println_ctr_20("Uh oh!");
        return;
    }

    #define T5i T5h + DT5 * 4
    if (tut_frame == T5i) {
        level_stop_trains();
        
        level_init();
        spmgr_clear_level();
        level_parse_ptr = (ubyte*)st_tut5_lev2;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        println("");
        st_println_ctr_20("Let's fix this!");

        return;
    }

    #define T5j T5i + DT5
    if (tut_frame == T5j) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T5k T5j + DT5
    if (tut_frame == T5k) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3)+4);
        return;
    }

    #define T5l T5k + 7
    if (tut_frame == T5l) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        return;
    }

    #define T5m T5l + 7
    if (tut_frame == T5m) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3)+4);
        return;
    }

    #define T5n T5m + 7
    if (tut_frame == T5n) {
        st_set_fake_cursor(BOARD_CTR_X(3), BOARD_CTR_Y(3));
        st_fake_dblclick_l();
        return;
    }

    #define T5o T5n + DT5
    if (tut_frame == T5o) {
        st_set_fake_cursor(336, 0);

        level_set_speed(0);
        level_start_trains();
        return;
    }

    #define T5p T5o + DT5 * 7
    if (tut_frame == T5p) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("That's better! Note");
        st_println_ctr_20("that COLLISIONS also");
        st_println_ctr_20("go from TWO trains");
        st_println_ctr_20("to TWO trains");

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define T5q T5p + 1
    if (tut_frame == T5q) {
        st_hide_btn_next();

        level_init();
        spmgr_clear_level();
        board_clear(board);
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 5, 22, 7);
        println("");
        st_println_ctr_20("One last note:");
        st_println_ctr_20("you can only mix");
        st_println_ctr_20("the PRIMARY colours");

        return;
    }

    #define T5r T5q + DT5 * 3
    if (tut_frame == T5r) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(3, 14, 24, 6);
        println_ctr("Any other combination", 22);
        println_ctr("of colours will", 22);
        println_ctr("produce a GARBAGE", 22);
        println_ctr("useless (brown) train", 22);

        return;
    }

    #define T5s T5r + DT5 * 3
    if (tut_frame == T5s) {
        // print_set_symbol(BLOCK_OUTER_BORDER);
        // print_frame_filled(2, 22, 26, 7);
        // print_set_pos(3,24);
        // print_str("RED    + YELLOW = ORANGE\n");
        // print_str("BLUE   + RED    = PURPLE\n");
        // print_str("YELLOW + BLUE   = GREEN\n");

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(2, 22, 26, 7);
        print_set_pos(3,24);
        print_set_color(CTEXT_RED);
        print_str("RED");
        print_set_color(CTEXT_WHI);
        print_str("    + ");
        print_set_color(CTEXT_YEL);
        print_str("YELLOW");
        print_set_color(CTEXT_WHI);
        print_str(" = ");
        print_set_color(CTEXT_ORA);
        print_str("ORANGE\n");
        print_set_color(CTEXT_BLU);
        print_str("BLUE");
        print_set_color(CTEXT_WHI);
        print_str("   + ");
        print_set_color(CTEXT_RED);
        print_str("RED");
        print_set_color(CTEXT_WHI);
        print_str("    = ");
        print_set_color(CTEXT_PUR);
        print_str("PURPLE\n");
        print_set_color(CTEXT_YEL);
        print_str("YELLOW");
        print_set_color(CTEXT_WHI);
        print_str(" + ");
        print_set_color(CTEXT_BLU);
        print_str("BLUE");
        print_set_color(CTEXT_WHI);
        print_str("   = ");
        print_set_color(CTEXT_GRE);
        print_str("GREEN\n");

        st_init_btn_next();
        inc_frame = 0;

        return;
    }

    #define T5t T5s + 1
    if (tut_frame == T5t) {
        st_hide_btn_next();

        level_init();
        spmgr_clear_level();
        level_parse_ptr = (ubyte*)st_tut5_lev3;
        level_parse_rawdef();
        level_populate_fixed();

        board[13] = NSNS;
        board[20] = NSNS;
        board[22] = WEWE;
        board[23] = WEWE;
        board[24] = WEWE;
        board[25] = WEWE;
        board[26] = WEWE;
        board[27] = NWWS;
        board[34] = NSNS;
        board[41] = NSNS;
        board_draw();

        return;
    }

    #define T5u T5t + DT5
    if (tut_frame == T5u) {
        level_set_speed(0);
        level_start_trains();
        return;
    }

    #define T5v T5u + DT5 * 5
    if (tut_frame == T5v) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 14, 6);
        println("");
        println_ctr("Yuck!", 12);

        tut_frame ++;
        inc_frame = 0;

        st_hide_btn_skip();
        st_hide_btn_next();
        st_init_btn_again();
        st_init_btn_puzzle();

        st_mark_tutorial_watched();
        return;
    }

}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######                         #       #######
//     #     #    #   #####         #    #  #
//     #     #    #     #           #    #  #
//     #     #    #     #           #######  #####
//     #     #    #     #                #        #
//     #     #    #     #                #  #     #
//     #      ####      #                #   #####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define DT6  50
#define DT62 32

__asm
_st_tut6_lev:
    DEFB $F8, $03, $13
    DEFB $BA, $33, $14
    DEFB $E2, $63, $14
    DEFB $00
__endasm;

extern ubyte* st_tut6_lev[];

__asm
_st_tut6_lev2:
    DEFB $F8, $04, $12
    DEFB $E2, $64, $13
    DEFB $B6, $34, $13
    DEFB $E8, $05, $13
    DEFB $F2, $65, $15
    DEFB $00
__endasm;

extern ubyte* st_tut6_lev2[];

ubyte st_tut6_tally0;
ubyte st_tut6_tally1;
ubyte st_tut6_tally2;

void st_update_45()
{
    st_update_common();

    if (tut_frame == 0) {
        st_init_btn_skip();

        st_tut6_tally0 = spmgr_curr_idx;
        spmgr_setup_tally(112, 104, (CTEXT_YEL * 16 | ATTR__Y__));
        spmgr_hide_tally(st_tut6_tally0);

        st_tut6_tally1 = spmgr_curr_idx;
        spmgr_setup_tally(96, 108, (CTEXT_YEL * 16 | ATTR__Y__));
        spmgr_hide_tally(st_tut6_tally1);

        st_tut6_tally2 = spmgr_curr_idx;
        spmgr_setup_tally(126, 108, (CTEXT_YEL * 16 | ATTR__Y__));
        spmgr_hide_tally(st_tut6_tally2);

        spmgr_end_fixed_sprites();

        level_parse_ptr = (ubyte*)st_tut6_lev;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        return;
    }

    #define          T60 25
    if (tut_frame == T60) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        println("");
        st_println_ctr_20("This is a");
        st_println_ctr_20("PAINTER piece");
        spmgr_show_tally(st_tut6_tally0);

        return;
    }

    #define          T61 T60 + DT62 * 2
    if (tut_frame == T61) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 24, 22, 5);
        st_println_ctr_20("See if you can");
        st_println_ctr_20("guess what it's");
        st_println_ctr_20("going to do");

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define          T62 T61 + 1
    if (tut_frame == T62) {
        st_hide_btn_next();
        spmgr_hide_tally(st_tut6_tally0);
        board_draw();

        st_set_fake_cursor(BOARD_CTR_X(0), BOARD_CTR_Y(3));
        st_fake_press_l();
        return;
    }

    #define          T63 T62 + DT62
    if (tut_frame == T63) {
        st_set_fake_target(BOARD_CTR_X(6), BOARD_CTR_Y(3));
        return;
    }

    #define          T64 T63 + DT62 * 6
    if (tut_frame == T64) {
        st_fake_release_l();
        return;
    }

    #define          T65 T64 + DT62
    if (tut_frame == T65) {
        st_set_fake_cursor(336, 0);  // offscreen
        return;
    }

    #define          T66 T65 + DT62
    if (tut_frame == T66) {
        level_set_speed(0);
        level_start_trains();
        return;
    }

    #define          T67 T66 + DT62 * 7
    if (tut_frame == T67) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        println("");
        st_println_ctr_20("Pretty simple,");
        st_println_ctr_20("right?");

        return;
    }

    #define          T68 T67 + DT62 * 2
    if (tut_frame == T68) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 22, 22, 6);
        st_println_ctr_20("There are a couple");
        st_println_ctr_20("more things you");
        st_println_ctr_20("should know about");
        st_println_ctr_20("PAINTER pieces");

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define          T69 T68 + 1
    if (tut_frame == T69) {
        st_hide_btn_next();

        board_draw();
        spmgr_show_tally(st_tut6_tally1);
        spmgr_show_tally(st_tut6_tally2);

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 6, 22, 6);
        st_println_ctr_20("The WHITE sides of");
        st_println_ctr_20("a PAINTER PIECE are");
        st_println_ctr_20("the ACTIVE sides");

        return;
    }

    #define T6A T69 + DT62 * 2
    if (tut_frame == T6A) {
        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 22, 22, 6);
        println("");
        st_println_ctr_20("Every PAINTER piece");
        st_println_ctr_20("has TWO ACTIVE sides");

        st_init_btn_next();
        inc_frame = 0;
        return;
    }

    #define T6B T6A + 1
    if (tut_frame == T6B) {
        st_hide_btn_next();

        level_stop_trains();

        board_draw();
        spmgr_hide_tally(st_tut6_tally1);
        spmgr_hide_tally(st_tut6_tally2);

        board_clear(board);
        level_init();
        spmgr_clear_level();
        level_parse_ptr = (ubyte*)st_tut6_lev2;
        level_parse_rawdef();
        level_populate_fixed();
        board_draw();

        print_set_symbol(BLOCK_OUTER_BORDER);
        print_frame_filled(4, 8, 22, 5);
        st_println_ctr_20("Trains can enter");
        st_println_ctr_20("either of the");
        st_println_ctr_20("ACTIVE sides");

        return;
    }

    #define T6C T6B + DT62
    if (tut_frame == T6C) {
        st_set_fake_cursor(BOARD_CTR_X(6), BOARD_CTR_Y(5));
        st_fake_press_l();
        return;
    }

    #define T6D T6C + DT62
    if (tut_frame == T6D) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(5));
        return;
    }

    #define T6E T6D + DT62
    if (tut_frame == T6E) {
        st_set_fake_target(BOARD_CTR_X(5), BOARD_CTR_Y(6));
        return;
    }

    #define T6F T6E + DT62
    if (tut_frame == T6F) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(6));
        return;
    }

    #define T6G T6F + DT62 * 2
    if (tut_frame == T6G) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(4));
        return;
    }

    #define T6H T6G + DT62 * 2
    if (tut_frame == T6H) {
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(4));
        return;
    }

    #define T6I T6H + DT62
    if (tut_frame == T6I) {
        st_set_fake_target(BOARD_CTR_X(2), BOARD_CTR_Y(5));
        return;
    }

    #define T6J T6I + DT62
    if (tut_frame == T6J) {
        st_set_fake_target(BOARD_CTR_X(0), BOARD_CTR_Y(5));
        return;
    }

    #define T6K T6J + DT62 * 2
    if (tut_frame == T6K) {
        st_set_fake_target(BOARD_CTR_X(0), BOARD_CTR_Y(4));
        return;
    }

    #define T6L T6K + DT62
    if (tut_frame == T6L) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(4));
        return;
    }

    #define T6M T6L + DT62 * 3
    if (tut_frame == T6M) {
        st_set_fake_target(BOARD_CTR_X(3), BOARD_CTR_Y(5));
        return;
    }

    #define T6N T6M + DT62
    if (tut_frame == T6N) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(5));
        return;
    }

    #define T6O T6N + DT62 
    if (tut_frame == T6O) {
        st_set_fake_target(BOARD_CTR_X(4), BOARD_CTR_Y(4));
        return;
    }

    #define T6P T6O + DT62
    if (tut_frame == T6P) {
        st_set_fake_target(BOARD_CTR_X(6), BOARD_CTR_Y(4));
        return;
    }

    #define T6Q T6P + DT62 * 2
    if (tut_frame == T6Q) {
        st_fake_release_l();
        st_set_fake_cursor(336, 0);  // offscreen
        level_set_speed(0);
        level_start_trains();
        return;
    }

    #define T6R T6Q + DT62 * 10
    if (tut_frame == T6R) {
        tut_frame ++;
        inc_frame = 0;

        st_hide_btn_skip();
        st_hide_btn_next();
        st_init_btn_again();
        st_init_btn_puzzle();

        st_mark_tutorial_watched();
        return;
    }
}




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////




