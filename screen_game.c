#include "screen_game.h"
#include "params.h"
#include "colors.h"
#include "level.h"
#include "print_tile.h"
#include "gui.h"
#include "board.h"
#include "update_build_version.h"
#include "design.h"
#include "io.h"
#include "mouse.h"
#include "spmgr.h"
#include "screen_controller.h"
#include "screen_main.h"
#include "screen_levels.h"
#include "screen_epilog.h"
#include "player_data.h"
#include <input.h>


ubyte sg_level_idx = 0;
ubyte sg_prov_idx = 0;
ubyte sg_score_earned = 0;
ubyte sg_prev_level_state = LEV_STATE_DESIGN;

ubyte sg_board[BOARD_SZ];

#define SG_SX 35
#define SG_SY0 0
#define SG_SY1 1

void sg_draw_score()
{
    print_set_color(CTEXT_GRE);
    // print_set_pos(SG_SX, SG_SY0);
    // print_str("SCORE");
    ubyte dx = 0;
    if (pd_score < 100) dx++;
    if (pd_score <  10) dx++;
    print_set_pos(SG_SX + dx, SG_SY1);
    print_dec_word(pd_score);
    print_block(BLOCK_STAR);
}


///////////////////////////////////////////////////////////////////////////////
//

#define SG_BX 30
#define SG_BY 2
#define SG_BW 10
#define SG_BH 3

///////////////////////////////////////////////////////////////////////////////
//
// BACK BUTTON
//

#define BK_BX 0
#define BK_BY 0
#define BK_BW 8
#define BK_BH 3

GuiButton sg_back_btn;

void sg_init_back_btn()
{
    print_set_color(CTEXT_WHI);
    GUIBUTTON_INIT(sg_back_btn, BK_BX, BK_BY, BK_BW, BK_BH);
    guibutton_draw(&sg_back_btn);
    print_set_pos(BK_BX + 1, BK_BY + 1);
    print_str("VOLVER");
}

void sg_fix_back_btn()
{
    print_set_color(CTEXT_WHI);
    print_set_pos  (BK_BX            , BK_BY + BK_BH - 1);
    print_block    (BLOCK_FIX1);
    print_set_pos  (BK_BX + BK_BW - 1, BK_BY + BK_BH - 1);
    print_block    (BLOCK_FIX2);
}

void sg_on_btn_back()
{
    if (!pd_levels_solved[sg_level_idx])
        pd_save_level_board(sg_level_idx, board);
    sc_switch_screen(sl_enter, sl_update, NULL);
}

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// STATUS PANEL
//

GuiButton sg_btn_undo;
GuiButton sg_btn_delete;

#define ST_UX SG_BX
#define ST_UY SG_BY+10
#define ST_UW SG_BW
#define ST_UH 3

void sg_init_undo_clear()
{
    GUIBUTTON_INIT(sg_btn_undo,   ST_UX, ST_UY,   ST_UW, ST_UH);
    GUIBUTTON_INIT(sg_btn_delete, ST_UX, ST_UY+3, ST_UW, ST_UH);
}

void sg_clear_level();

void sg_check_undo_delete()
{
    // if (!mouse_pressed_l)
    //     return;

}


//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// STATUS PANEL
//

GuiButton sg_status_panel;

#define ST_BX SG_BX
#define ST_BY SG_BY+10
#define ST_BW SG_BW
#define ST_BH 6

void sg_init_status_panel()
{
    GUIBUTTON_INIT(sg_status_panel, ST_BX, ST_BY, ST_BW, ST_BH);
}

void sg_update_undo_delete()
{
    guibutton_draw(&sg_btn_undo);
    print_set_color(CTEXT_WHI);
    print_set_pos(ST_UX+1, ST_UY+1);
    print_str("DESHACER");

    guibutton_draw(&sg_btn_delete);
    print_set_color(CTEXT_RED);
    print_set_pos(ST_UX+2, ST_UY+4);
    print_str("BORRAR");
}

void sg_update_status_panel()
{
    if (level_state == LEV_STATE_DESIGN)
        return;

    print_set_color(CTEXT_WHI);
    guibutton_draw_fine(&sg_status_panel);
    print_set_pos(ST_BX + 2, ST_BY + 2);
    print_str("ESTADO");

    if (level_state == LEV_STATE_RUNNING) {
        print_set_color(CTEXT_GRE);
        print_set_pos(ST_BX + 4, ST_BY + 3);
        print_str("OK");
        return;
    }

    if (level_state == LEV_STATE_CRASHED) {
        print_set_color(CTEXT_RED);
        print_set_pos(ST_BX + 1, ST_BY + 3);
        print_str("COLISION");
        return;
    }

    if (level_state == LEV_STATE_FINISHED) {
        print_set_color(CTEXT_BLU);
        print_set_pos(ST_BX + 1, ST_BY + 3);
        print_str("SUPERADO");
        return;
    }
}

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// SPEED BUTTONS
//

#define SP_BX SG_BX
#define SP_BY SG_BY+4
#define SP_BWB 3
#define SP_BHB 3

GuiButton sg_incvel_btn;
GuiButton sg_decvel_btn;

void sg_less_speed();
void sg_more_speed();
void sg_update_speed_value();

void sg_init_vel_btns()
{
    GUIBUTTON_INIT(sg_decvel_btn, SP_BX+1, SP_BY+2, SP_BWB, SP_BHB);
    GUIBUTTON_INIT(sg_incvel_btn, SP_BX+6, SP_BY+2, SP_BWB, SP_BHB);
    guibutton_draw(&sg_decvel_btn);
    guibutton_draw(&sg_incvel_btn);
    print_set_color(CTEXT_WHI);
    print_set_pos(SP_BX+2, SP_BY+3);
    print_str("-");
    print_set_pos(SP_BX+1, SP_BY+1);
    print_str("RAPIDEZ:");
    print_set_pos(SP_BX+7, SP_BY+3);
    print_str("+");
    sg_update_speed_value();
}

void sg_update_speed_value()
{
    print_set_pos(SP_BX+4, SP_BY+3);
    if (level_speed < 0) {
        print_set_color(CTEXT_BLU);
        print_char('-');
        print_hex_nibble(-level_speed);
    }
    else {
        print_set_color(CTEXT_ORA);
        print_char('+');
        print_hex_nibble(level_speed);
    }
}

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// START / STOP BUTTON
//

#define SS_BX SG_BX
#define SS_BY SG_BY
#define SS_BW SG_BW
#define SS_BH 4

GuiButton sg_ss_btn;

void sg_init_ss_btn()
{
    GUIBUTTON_INIT(sg_ss_btn, SS_BX, SS_BY, SS_BW, SS_BH);
    guibutton_draw(&sg_ss_btn);
}

void sg_update_ss_state()
{
    if (level_state == LEV_STATE_DESIGN) {
        print_set_color(CTEXT_WHI);
        print_set_pos(SS_BX + 1, SS_BY + 1);
        print_str("ARRANCAR");
        print_set_pos(SS_BX + 1, SS_BY + 2);
        print_str(" TRENES ");

        guibutton_clear(&sg_status_panel);
        sg_update_undo_delete();
    }
    else {
        print_set_color(CTEXT_WHI);
        print_set_pos(SS_BX + 1, SS_BY + 1);
        print_str("VOLVER A");
        print_set_pos(SS_BX + 1, SS_BY + 2);
        print_str(" DIBUJO ");

        guibutton_clear(&sg_status_panel);
        sg_update_status_panel();
    }
}

void sg_toggle_start_stop()
{
    if (level_state == LEV_STATE_DESIGN) {
        board_clear(sg_board);
        board_copy_tracks(board, sg_board);
        level_start_trains();
    }
    else {
        level_stop_trains();
        level_load(0, sg_level_idx);
        board_copy_tracks(sg_board, board);
        board_draw();
    }
    sg_update_ss_state();
}

void sg_check_ss_btn()
{
    // if (!mouse_pressed_l)
    //     return;

}

//
///////////////////////////////////////////////////////////////////////////////

void initialize_level()
{
    print_set_color(CTEXT_WHI);

    // load saved level here in sg_board
    pd_load_level_board(sg_level_idx, sg_board);

    level_load(0, sg_level_idx);
    level_set_state(LEV_STATE_DESIGN);

    board_copy_tracks(sg_board, board);
    board_draw();

    sg_prov_idx = lev_prov_idx;

    print_set_color(CTEXT_WHI);
    print_set_pos(9, 1);
    //             Dousing The Flame 10*
    print_str("                      ");
    print_set_pos(9, 1);
    print_str(lev_name);
    print_char(' ');

    ubyte levsolved = pd_levels_solved[sg_level_idx];
    if (levsolved) print_set_color(CTEXT_GRE);
    else           print_set_color(CTEXT_YEL);

    print_dec_byte(lev_stars);
    print_block(BLOCK_STAR);
    print_set_color(CTEXT_WHI);
}

void sg_clear_level()
{
    if (level_state != LEV_STATE_DESIGN) return;

    pd_snapshot();

    // level_load(0, sg_level_idx);
    board_clear_tracks(board);
    board_draw();
}

void sg_prev_level()
{
    if (level_state != LEV_STATE_DESIGN) return;
    pd_clear_undo();
    board_clear(board); //?
    sg_level_idx--;
    if (sg_level_idx > LEV_MAX)
        sg_level_idx = 0;
    initialize_level();
}

void sg_next_level()
{
    if (level_state != LEV_STATE_DESIGN) return;
    pd_clear_undo();
    board_clear(board); //?
    sg_level_idx++;
    if (sg_level_idx > LEV_MAX)
        sg_level_idx = LEV_MAX;
    initialize_level();
}

void sg_less_speed()
{
    if (level_speed <= -6) return;
    level_set_speed(level_speed - 1);
    sg_update_speed_value();
}

void sg_more_speed()
{
    if (level_speed >= 5) return;
    level_set_speed(level_speed + 1);
    sg_update_speed_value();
}

///////////////////////////////////////////////////////////////////////////////

ubyte sg_epilog_invoked;

GuiButton sg_board_rect;
void sg_on_board_rect() {}

void sg_enter()
{
    gui_clear_screen();
    sg_draw_score();
    sg_init_back_btn();
    board_draw_frame();
    sg_fix_back_btn();
    sg_init_status_panel();
    sg_init_undo_clear();
    sg_init_vel_btns();

    pd_clear_undo();

    design_init();

    initialize_level();

    if (pd_debug_on) {
        print_set_color(CTEXT_WHI);
        print_set_pos(30, 31);
        print_str(BUILD_VER);
        // print_set_pos(30, 29);
        // print_str(BUILD_DATE);
        // print_set_pos(30, 30);
        // print_str(BUILD_TIME);
    }

    sg_init_ss_btn();
    sg_update_ss_state();
    sg_prev_level_state = LEV_STATE_DESIGN;

    GUIBUTTON_INIT(sg_board_rect, BOARD_X, BOARD_Y, BOARD_W*4, BOARD_H*4);

    sg_epilog_invoked = 0;
}

ubyte debug_mode = 0xFF;
void update_debug();

ubyte prev_inkey = 0;

void sg_keyb_update()
{
    ubyte key = in_inkey();
    if (key == prev_inkey) return;

    // print_set_color(CTEXT_WHI);
    // print_set_pos(0, 0);
    // print_dec_byte(key);
    // print_str("     ");

    if (!pd_debug_on) {
        switch(key) {
            case 'u': sg_less_speed(); break;
            case 'i': sg_more_speed(); break;
            case 13: sg_toggle_start_stop(); break;
        }
    }
    else {
        switch(key) {
            case 'u': sg_less_speed(); break;
            case 'i': sg_more_speed(); break;
            case 13: sg_toggle_start_stop(); break;

            case 'c': sg_clear_level(); break;
            case 'k': sg_prev_level(); break;
            case 'l': sg_next_level(); break;
            case 'e': io_board_store(); break;
            case 'r': io_board_recall(); break;

            // case '9': debug_mode = 0; break;
            // case '1': debug_mode = 1; break;
            // case '2': debug_mode = 2; break;
            // case '3': debug_mode = 3; break;
            // case '4': debug_mode = 4; break;
            // case '5': debug_mode = 5; break;
            // case '6': debug_mode = 6; break;
        }
    }



    prev_inkey = key;
}

void sg_update()
{
    mouseAction = nullMouseAction;
    if      (GBMI(&sg_board_rect)) mouseAction = sg_on_board_rect;
    else if (GBMI(&sg_back_btn)  ) mouseAction = sg_on_btn_back;
    else if (GBMI(&sg_ss_btn)    ) mouseAction = sg_toggle_start_stop;
    else if (GBMI(&sg_incvel_btn)) mouseAction = sg_more_speed;
    else if (GBMI(&sg_decvel_btn)) mouseAction = sg_less_speed;
    else if (level_state == LEV_STATE_DESIGN) {
        if      (GBMI(&sg_btn_undo)  ) mouseAction = pd_undo;
        else if (GBMI(&sg_btn_delete)) mouseAction = sg_clear_level;
    }

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    if (sg_epilog_invoked)
        return;

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    sg_keyb_update();

    if (pd_debug_on) {
        gui_display_train_frame();
        gui_display_train_steps();
    }

    level_update();

    design_update();

    // update_debug();

    if (sg_prev_level_state != level_state) {
        sg_prev_level_state  = level_state;
        sg_update_status_panel();
    }

    if (level_state == LEV_STATE_FINISHED) {
        if (!sg_epilog_invoked) {
            // mark level as solved
            if (pd_levels_solved[sg_level_idx]) {
                // already solved, nothing to be earned or updated
                sg_score_earned = 0;
            }
            else {
                sg_score_earned = lev_stars;
                pd_levels_solved[sg_level_idx] = 1;
                pd_update_score();
                sg_draw_score();
            }

            // save board
            pd_save_level_board(sg_level_idx, sg_board);

            // delayed switch
            sc_switch_delay(50);
            if (sg_score_earned && pd_score == STARS_IN_BONUS_LEVELS) {
                // double victory screen when first time solved bonus levels
                sc_switch_screen(sw_enter, sw_update, NULL);
            }
            else if (sg_score_earned && pd_score == STARS_IN_BASE_LEVELS) {
                // victory screen when first time solved base levels
                sc_switch_screen(sv_enter, sv_update, NULL);
            }
            else {
                // epilog screen else
                sc_switch_screen(se_enter, se_update, NULL);
            }

            // set epilog invoked flag
            sg_epilog_invoked = 1;
        }
    }
}

/*

#include <arch/zxn/esxdos.h>
#include <errno.h>

extern ubyte end_of_code;
extern ubyte start_of_code;

void update_debug_mem_usage()
{
    // ubyte x = 30;
    // ubyte y = 2;
    // print_set_color(CTEXT_WHI);

    // print_set_pos(x, y);
    // print_str("MEM USAGE");
    // y += 2;

    // print_set_pos(x, y);
    // print_str("CRT START");
    // y += 1;
    // print_set_pos(x, y);
    // print_str("  $8000");
    // y += 2;

    // print_set_pos(x, y);
    // print_str("CODE START");
    // y += 1;
    // print_set_pos(x, y);
    // print_str("  $");
    // print_hex_word(&start_of_code);
    // y += 2;

    // print_set_pos(x, y);
    // print_str("CODE END");
    // y += 1;
    // print_set_pos(x, y);
    // print_str("  $");
    // print_hex_word(&end_of_code);
    // y += 2;

    // __asm
    // PUSH HL
    // LD HL, SP
    // LD (_param0w), HL
    // POP HL
    // __endasm

    // print_set_pos(x, y);
    // print_str("STACK TOP");
    // y += 1;
    // print_set_pos(x, y);
    // print_str("  $");
    // print_hex_word(param0w);
    // y += 2;

    // print_set_pos(x, y);
    // print_str("USED SPACE");
    // y += 1;
    // print_set_pos(x, y);
    // print_str("  $");
    // print_hex_word(&end_of_code - 0x8000);
    // y += 2;

    // print_set_pos(x, y);
    // print_str("REMAINING");
    // y += 1;
    // print_set_pos(x, y);
    // print_str("  $");
    // print_hex_word(param0w - &end_of_code);
    // y += 2;

    // print_set_pos(x, y);
    // print_str("ERRNO:");
    // print_hex_word(errno);
    // y += 2;

    // print_set_pos(x, y);
    // print_str("ESX_OPEN");
    // y += 1;
    // print_set_pos(x, y);
    // print_str("  $");
    // print_hex_word(&esxdos_f_open);
    // y += 2;

}

void update_debug_param_usage()
{
// #define update_debug_param_usage_ENABLED
//     ubyte x = 30;
//     ubyte y = 2;
//     print_set_color(CTEXT_WHI);

//     print_set_pos(x, y);
//     print_str("PARAMETERS");
// #ifndef  update_debug_param_usage_ENABLED
//     print_set_pos(x, ++y);
//     print_str("DISABLED");
// #else
//     y += 2;

//     print_set_pos(x, y++); print_str("P0B $"); print_hex_byte(param0b);
//     print_set_pos(x, y++); print_str("P1B $"); print_hex_byte(param1b);
//     print_set_pos(x, y++); print_str("P2B $"); print_hex_byte(param2b);
//     print_set_pos(x, y++); print_str("P3B $"); print_hex_byte(param3b);
//     print_set_pos(x, y++); print_str("P4B $"); print_hex_byte(param4b);
//     print_set_pos(x, y++); print_str("P5B $"); print_hex_byte(param5b);
//     print_set_pos(x, y++); print_str("P6B $"); print_hex_byte(param6b);
//     print_set_pos(x, y++); print_str("P7B $"); print_hex_byte(param7b);
//     y++;

//     print_set_pos(x, y++); print_str("P0W $"); print_hex_word(param0w);
//     print_set_pos(x, y++); print_str("P1W $"); print_hex_word(param1w);
//     print_set_pos(x, y++); print_str("P2W $"); print_hex_word(param2w);
//     print_set_pos(x, y++); print_str("P3W $"); print_hex_word(param3w);
//     print_set_pos(x, y++); print_str("P4W $"); print_hex_word(param4w);
//     print_set_pos(x, y++); print_str("P5W $"); print_hex_word(param5w);
//     print_set_pos(x, y++); print_str("P6W $"); print_hex_word(param6w);
//     print_set_pos(x, y++); print_str("P7W $"); print_hex_word(param7w);
// #endif
}

void update_debug_tile_types()
{
// #define update_debug_tile_types_ENABLED
//     ubyte x = 30;
//     ubyte y = 2;
//     print_set_color(CTEXT_WHI);

//     print_set_pos(x, y);
//     print_str("TILE TYPES");
// #ifndef  update_debug_tile_types_ENABLED
//     print_set_pos(x, ++y);
//     print_str("DISABLED");
// #else
//     y += 2;

//     print_set_pos(x, y++);
//     print_str("I T S F");
//     print_set_pos(x, y++);
//     print_str("D R N X");
//     print_set_pos(x, y++);
//     print_str("X K K D");
//     y++;

//     for (ubyte i = 0; i < 16; i++) {
//         print_set_pos(x, y++);
//         print_hex_nibble(i);
//         print_char(' ');
//         print_hex_nibble(tile_is_track(i << 4));
//         print_char(' ');
//         print_hex_nibble(tile_is_sink(i << 4));
//         print_char(' ');
//         print_hex_nibble(tile_is_fixed(i << 4));
//     }

// #endif
}

void update_debug()
{
    if (debug_mode == 0xFF)
        return;

    print_set_color(CTEXT_WHI);
    switch (debug_mode) {
        case 0:
            gui_clear_side_panel();
            debug_mode = 0xFF;
            return;

        // case 1:
        //     gui_clear_side_panel();
        //     debug_mode = 0x81;
        //     break;

        // case 2:
        //     gui_clear_side_panel();
        //     debug_mode = 0x82;
        //     break;

        // case 3:
        //     gui_clear_side_panel();
        //     debug_mode = 0x83;
        //     break;

        // case 4:
        //     gui_clear_side_panel();
        //     update_debug_tile_types();
        //     debug_mode = 0xFF;
        //     break;

        // case 5:
        //     gui_clear_side_panel();
        //     debug_mode = 0x85;
        //     break;

        case 6:
            gui_clear_side_panel();
            debug_mode = 0x86;
            break;

        // case 0x81:
        //     update_debug_mem_usage();
        //     break;

        // case 0x82:
        //     mouse_print_debug();
        //     break;

        // case 0x83:
        //     update_debug_param_usage();
        //     break;

        // case 0x85:
        //     design_print_debug();
        //     break;

        case 0x86:
            spmgr_print_debug();
        
    }
}

*/
