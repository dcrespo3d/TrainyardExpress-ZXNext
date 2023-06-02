#include "types.h"
uword dummy_uword_in_zero_address = 0xCAFE;

// void p0_p0_pagein()
// {
//     __asm__("NEXTREG $50, 36");
//     __asm__("NEXTREG $51, 37");
// }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #     #
//  ##   ##    ##       #    #    #
//  # # # #   #  #      #    ##   #
//  #  #  #  #    #     #    # #  #
//  #     #  ######     #    #  # #
//  #     #  #    #     #    #   ##
//  #     #  #    #     #    #    #
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "screen_main.h"
#include "gui.h"
#include "spmgr.h"
#include "mouse.h"
#include "colors.h"
#include "screen_controller.h"
#include "screen_provs.h"
#include "screen_game.h"
#include "screen_epilog.h"
#include "print_tile.h"
#include "player_data.h"
#include "sound.h"
#include "keyb.h"
#include "board.h"
#include "params.h"


#define SM_ST_X 10
#define SM_ST_Y 11
#define SM_ST_W 20
#define SM_ST_H 5

#define SM_ST2_X 10
#define SM_ST2_Y 10
#define SM_ST2_W 9
#define SM_ST2_H 6

#define SM_BP_X 19
#define SM_BP_Y 10
#define SM_BP_W 11
#define SM_BP_H 6

#define SM_AB_X 10
#define SM_AB_Y 16
#define SM_AB_W 8
#define SM_AB_H 5

#define SM_OP_X 18
#define SM_OP_Y 16
#define SM_OP_W 12
#define SM_OP_H 5

GuiButton sm_btn_start;
GuiButton sm_btn_bonus;
GuiButton sm_btn_opts;
GuiButton sm_btn_about;

const char* str_TRAINYARD_EXPRESS = "TRAINYARD EXPRESS";
const char* str_DEVELOPMENT_BUILD = "DEVELOPMENT BUILD";
const char* str_ABOUT = "INFO";
const char* str_OPTIONS = "OPCIONES";

ubyte sm_show_bonus_btn;

void si_init_trains();
void si_update_trains();

void p0_sm_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    si_init_trains();

    sm_show_bonus_btn = 0;
    if (pd_all_levels_unlocked || pd_base_levels_solved) 
        sm_show_bonus_btn = 1;

    print_set_color(COLOR_WHI);
    #define TITLE_TILE 60
    for (ubyte i = 0; i < 4; i++) {
        draw_tile_xy(12 + 4*i, 5, TITLE_TILE + i);
    }

    print_set_color(CTEXT_WHI);
    // print_set_pos(11, 8);
    // print_str(str_TRAINYARD_EXPRESS);

    if (!sm_show_bonus_btn) {
        print_set_color(CTEXT_GRE);
        GUIBUTTON_INIT(sm_btn_start, SM_ST_X, SM_ST_Y, SM_ST_W, SM_ST_H);
        guibutton_draw(&sm_btn_start);
    
        print_set_color(CTEXT_WHI);
        print_set_pos(SM_ST_X + 3, SM_ST_Y + 2);
        print_str("EMPEZAR JUEGO!");
    }
    else {
        print_set_color(CTEXT_GRE);
        GUIBUTTON_INIT(sm_btn_start, SM_ST2_X, SM_ST2_Y, SM_ST2_W, SM_ST2_H);
        guibutton_draw(&sm_btn_start);
    
        print_set_color(CTEXT_WHI);
        print_set_pos(SM_ST2_X + 2, SM_ST2_Y + 2);
        print_str("JUGAR");
        print_set_pos(SM_ST2_X + 3, SM_ST2_Y + 3);
        print_str("YA!");

        print_set_color(CTEXT_BLU);
        GUIBUTTON_INIT(sm_btn_bonus, SM_BP_X, SM_BP_Y, SM_BP_W, SM_BP_H);
        guibutton_draw(&sm_btn_bonus);
    
        print_set_color(CTEXT_WHI);
        print_set_pos(SM_BP_X + 2, SM_BP_Y + 2);
        print_str("PUZZLES");
        print_set_pos(SM_BP_X + 3, SM_BP_Y + 3);
        print_str("EXTRA");
    }

    print_set_color(CTEXT_WHI);
    GUIBUTTON_INIT(sm_btn_opts, SM_OP_X, SM_OP_Y, SM_OP_W, SM_OP_H);
    guibutton_draw(&sm_btn_opts);
    print_set_pos(SM_OP_X + 2, SM_OP_Y + 2);
    print_str(str_OPTIONS);

    GUIBUTTON_INIT(sm_btn_about, SM_AB_X, SM_AB_Y, SM_AB_W, SM_AB_H);
    guibutton_draw(&sm_btn_about);
    print_set_pos(SM_AB_X + 2, SM_AB_Y + 2);
    print_str(str_ABOUT);
}

void sm_on_start()
{
    sc_switch_screen(sp_enter, sp_update, NULL);
}

void sm_on_opts()
{
    sc_switch_screen(so_enter, so_update, NULL);
}

void sm_on_about()
{
    sc_switch_screen(sa_enter, sa_update, NULL);
}

void sm_on_bonus()
{
    sc_switch_screen(sb_enter, sb_update, NULL);
}

void p0_sm_update()
{
    mouseAction = nullMouseAction;
    if      (guibutton_mouseinto(&sm_btn_start)) mouseAction = sm_on_start;
    else if (guibutton_mouseinto(&sm_btn_opts))  mouseAction = sm_on_opts;
    else if (guibutton_mouseinto(&sm_btn_about)) mouseAction = sm_on_about;
    else if (sm_show_bonus_btn) {
         if (guibutton_mouseinto(&sm_btn_bonus)) mouseAction = sm_on_bonus;        
    }

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    si_update_trains();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  ######
//  #     #  #####    ####   #    #   ####
//  #     #  #    #  #    #  #    #  #
//  ######   #    #  #    #  #    #   ####
//  #        #####   #    #  #    #       #
//  #        #   #   #    #   #  #   #    #
//  #        #    #   ####     ##     ####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "screen_provs.h"
#include "gui.h"
#include "spmgr.h"
#include "mouse.h"
#include "colors.h"
#include "screen_controller.h"
#include "screen_levels.h"
#include "screen_game.h"
#include "screen_main.h"
#include "print_tile.h"
#include "player_data.h"
#include "level.h"
#include <string.h>

#define SP_COL0_X 0
#define SP_COL1_X 20
#define SP_COL_W 20
#define SP_ROW_Y 5
#define SP_ROW_H 4
#define SP_ROW_DY 5
#define SP_TXT_AVAIL 18

#define NUM_PROVS_IN_COL 5
#define NUM_PROVS 10

GuiButton sp_prov_btns[NUM_PROVS];
ubyte sp_prov_unlocked[NUM_PROVS];

extern GuiButton sg_back_btn;
extern void sg_init_back_btn();

const char* str_Provinces  = "Provincias";
const char* str_4_qmarks   = "?  ?  ?  ?";
const char* str__slash_    = " / ";
const char* str__to_unlock = " para abrir";

void sp_draw_sep_line()
{
    print_set_pos(0, 3);
    print_set_attr((CTEXT_GRE << 4) | ATTR___R_);
    print_set_symbol(BLOCK_FSTR);
    ubyte cnt = 40;
    while (cnt--)
        print_symbol();
}

void p0_sp_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    sp_draw_sep_line();
    sg_draw_score();
    sg_init_back_btn();

    print_set_color(CTEXT_WHI);
    print_set_pos(9, 1);
    print_str(str_Provinces);

    ubyte x, y;

    for (ubyte i = 0; i < NUM_PROVS; i++)
    {
        if (i < NUM_PROVS_IN_COL) {
            x = SP_COL0_X;
            y = SP_ROW_Y + i * SP_ROW_DY;
        }
        else {
            x = SP_COL1_X;
            y = SP_ROW_Y + (i - NUM_PROVS_IN_COL) * SP_ROW_DY;
        }

        GuiButton* btn = &sp_prov_btns[i];
        GUIBUTTON_INIT((*btn), x, y, SP_COL_W, SP_ROW_H);

        ubyte* provrec = provarr[i];

        uword ustars = *((uword*)provrec);
        const char* provname = (const char*)(provrec + 2);
        ubyte pnlen = strlen(provname);
        ubyte levcount = *(provrec + pnlen + 3);
        ubyte levidx = *(provrec + pnlen + 4);

        ubyte levactcount = 0;  // actual level count (no tutorials)
        ubyte levsolcount = 0;  // solved level count
        for (ubyte j = 0; j < levcount; j++) {
            ubyte* levrec = levarr[levidx+j];
            ubyte levstars = *levrec;
            if (!levstars) continue;

            levactcount++;
            ubyte levsolved = pd_levels_solved[levidx+j];
            if (!levsolved) continue;

            levsolcount++;
        }

        if (ustars <= pd_score || pd_all_levels_unlocked)
        {
            sp_prov_unlocked[i] = 1;

            print_set_color(CTEXT_WHI);
            guibutton_draw(btn);

            const char* provname = (const char*)(provrec + 2);
            ubyte dx = (SP_TXT_AVAIL - ((ubyte)strlen(provname))) >> 1;
            print_set_pos(btn->x+1+dx, btn->y+1);
            print_str(provname);

            print_set_pos(btn->x+7, btn->y+2);
            if (levsolcount < levactcount) print_set_color(CTEXT_YEL);
            else                           print_set_color(CTEXT_GRE);
            print_hex_nibble(levsolcount);

            print_str(str__slash_);
            print_hex_nibble(levactcount);
        }
        else
        {
            sp_prov_unlocked[i] = 0;
            
            print_set_color(CTEXT_ORA);
            guibutton_draw(btn);

            print_set_pos(btn->x+5, btn->y+1);
            print_str(str_4_qmarks);

            ustars -= pd_score;
            ubyte dx = 2;
            if (ustars < 100) dx++;
            if (ustars <  10) dx++;

            print_set_color(CTEXT_YEL);
            print_set_pos(btn->x+dx, btn->y+2);
            print_dec_word(ustars);
            print_block(BLOCK_STAR);
            print_str(str__to_unlock);
        }
    }
}

void sp_on_back_btn()
{
    sc_switch_screen(sm_enter, sm_update, NULL);
}

ubyte sp_over_prov_idx = 0;

void sp_on_prov_btn()
{
    sg_prov_idx = sp_over_prov_idx;
    sc_switch_screen(sl_enter, sl_update, NULL);
}

void p0_sp_update()
{
    mouseAction = nullMouseAction;

    if (guibutton_mouseinto(&sg_back_btn)) {
        mouseAction = sp_on_back_btn;
    }
    else for (ubyte i = 0; i < NUM_PROVS; i++) {
        if (!sp_prov_unlocked[i]) break;
        GuiButton* btn = &sp_prov_btns[i];
        if (guibutton_mouseinto(btn)) {
            sp_over_prov_idx = i;
            mouseAction = sp_on_prov_btn;
            break;
        }
    }

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
//  ######
//  #     #   ####   #    #  #    #   ####
//  #     #  #    #  ##   #  #    #  #
//  ######   #    #  # #  #  #    #   ####
//  #     #  #    #  #  # #  #    #       #
//  #     #  #    #  #   ##  #    #  #    #
//  ######    ####   #    #   ####    ####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "screen_provs.h"
#include "gui.h"
#include "spmgr.h"
#include "mouse.h"
#include "colors.h"
#include "screen_controller.h"
#include "screen_levels.h"
#include "screen_game.h"
#include "screen_main.h"
#include "print_tile.h"
#include "player_data.h"
#include "level.h"
#include <string.h>

#define SB_COL_X 10
#define SB_COL1_X 20
#define SB_COL_W 20
#define SB_ROW_Y 5
#define SB_ROW_H 4
#define SB_ROW_DY 5
#define SB_TXT_AVAIL 18

#define FIRST_BONUS_PROV 10
#define NUM_BONUS_PROVS 3

GuiButton sb_prov_btns[NUM_BONUS_PROVS];
ubyte sb_prov_unlocked[NUM_BONUS_PROVS];

extern GuiButton sg_back_btn;
extern void sg_init_back_btn();

// const char* str_4_qmarks   = "?  ?  ?  ?";
// const char* str__slash_    = " / ";
// const char* str__to_unlock = " to unlock";

void p0_sb_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    sp_draw_sep_line();
    sg_draw_score();
    sg_init_back_btn();

    print_set_color(CTEXT_WHI);
    print_set_pos(9, 1);
    print_str("Bonus Puzzles");

    for (ubyte i = 0; i < NUM_BONUS_PROVS; i++)
    {
        ubyte x = SB_COL_X;
        ubyte y = SB_ROW_Y + i * SB_ROW_DY;

        GuiButton* btn = &sb_prov_btns[FIRST_BONUS_PROV + i];
        GUIBUTTON_INIT((*btn), x, y, SB_COL_W, SB_ROW_H);

        ubyte* provrec = provarr[FIRST_BONUS_PROV + i];

        uword ustars = *((uword*)provrec);
        const char* provname = (const char*)(provrec + 2);
        ubyte pnlen = strlen(provname);
        ubyte levcount = *(provrec + pnlen + 3);
        ubyte levidx = *(provrec + pnlen + 4);

        ubyte levactcount = 0;  // actual level count (no tutorials)
        ubyte levsolcount = 0;  // solved level count
        for (ubyte j = 0; j < levcount; j++) {
            ubyte* levrec = levarr[levidx+j];
            ubyte levstars = *levrec;
            if (!levstars) continue;

            levactcount++;
            ubyte levsolved = pd_levels_solved[levidx+j];
            if (!levsolved) continue;

            levsolcount++;
        }

        if (ustars <= pd_score || pd_all_levels_unlocked)
        {
            sb_prov_unlocked[FIRST_BONUS_PROV + i] = 1;
            
            print_set_color(CTEXT_WHI);
            guibutton_draw(btn);

            const char* provname = (const char*)(provrec + 2);
            ubyte dx = (SB_TXT_AVAIL - ((ubyte)strlen(provname))) >> 1;
            print_set_pos(btn->x+1+dx, btn->y+1);
            print_str(provname);

            print_set_pos(btn->x+7, btn->y+2);
            if (levsolcount < levactcount) print_set_color(CTEXT_YEL);
            else                           print_set_color(CTEXT_GRE);
            print_hex_nibble(levsolcount);

            print_str(str__slash_);
            print_hex_nibble(levactcount);
        }
        else
        {
            sb_prov_unlocked[FIRST_BONUS_PROV + i] = 0;

            print_set_color(CTEXT_ORA);
            guibutton_draw(btn);

            print_set_pos(btn->x+5, btn->y+1);
            print_str(str_4_qmarks);

            ustars -= pd_score;
            ubyte dx = 2;
            if (ustars < 100) dx++;
            if (ustars <  10) dx++;

            print_set_color(CTEXT_YEL);
            print_set_pos(btn->x+dx, btn->y+2);
            print_dec_word(ustars);
            print_block(BLOCK_STAR);
            print_str(str__to_unlock);
        }
    }
}

void sb_on_back_btn()
{
    sc_switch_screen(sm_enter, sm_update, NULL);
}

ubyte sb_over_prov_idx = 0;

void sb_on_prov_btn()
{
    sg_prov_idx = sb_over_prov_idx;
    sc_switch_screen(sl_enter, sl_update, NULL);
}

void p0_sb_update()
{
    mouseAction = nullMouseAction;

    if (guibutton_mouseinto(&sg_back_btn)) {
        mouseAction = sb_on_back_btn;
    }
    else for (ubyte i = 0; i < NUM_BONUS_PROVS; i++) {
        if (!sb_prov_unlocked[FIRST_BONUS_PROV + i]) break;
        GuiButton* btn = &sb_prov_btns[FIRST_BONUS_PROV + i];
        if (guibutton_mouseinto(btn)) {
            sb_over_prov_idx = FIRST_BONUS_PROV + i;
            mouseAction = sb_on_prov_btn;
            break;
        }
    }

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
//  #
//  #        ######  #    #  ######  #        ####
//  #        #       #    #  #       #       #
//  #        #####   #    #  #####   #        ####
//  #        #       #    #  #       #            #
//  #        #        #  #   #       #       #    #
//  #######  ######    ##    ######  ######   ####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define SL_COL0_X 0
#define SL_COL1_X 20
#define SL_COL_W 20
#define SL_ROW_Y 5
#define SL_ROW_H 4
#define SL_ROW_DY 5
#define SL_TXT_AVAIL 18

#define MAX_LEVELS_IN_PROV 8
#define NUM_LEVELS_IN_COL 4

GuiButton sl_level_btns[MAX_LEVELS_IN_PROV];

extern GuiButton sg_back_btn;
extern void sg_init_back_btn();

ubyte* sl_provrec;
uword sl_ustars;
const char* sl_provname;
ubyte sl_pnlen;
ubyte sl_levcount;
ubyte sl_levidx;

void p0_sl_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    sp_draw_sep_line();
    sg_init_back_btn();
    sg_draw_score();

    print_set_color(CTEXT_WHI);

    sl_provrec = provarr[sg_prov_idx];
    sl_ustars = *((uword*)sl_provrec);
    sl_provname = (const char*)(sl_provrec + 2);
    sl_pnlen = strlen(sl_provname);
    sl_levcount = *(sl_provrec + sl_pnlen + 3);
    sl_levidx = *(sl_provrec + sl_pnlen + 4);

    print_set_pos(9, 1);
    print_str(sl_provname);

    ubyte x, y;

    for (ubyte i = 0; i < sl_levcount; i++)
    {
        if (i < NUM_LEVELS_IN_COL) {
            x = SL_COL0_X;
            y = SL_ROW_Y + i * SL_ROW_DY;
        }
        else {
            x = SL_COL1_X;
            y = SL_ROW_Y + (i - NUM_LEVELS_IN_COL) * SL_ROW_DY;
        }

        GuiButton* btn = &sl_level_btns[i];
        GUIBUTTON_INIT((*btn), x, y, SL_COL_W, SL_ROW_H);

        ubyte* levrec = levarr[sl_levidx + i];

        ubyte levstars = *levrec;
        const char* levname = (const char*)(levrec + 2);

        ubyte levsolved = pd_levels_solved[sl_levidx + i];

        ubyte dx = (SL_TXT_AVAIL - ((ubyte)strlen(levname))) >> 1;
        print_set_pos(btn->x+1+dx, btn->y+1);

        if (levstars != 0) {
            print_set_color(CTEXT_WHI);
            print_str(levname);
            guibutton_draw(btn);

            if (!levsolved) {
                print_set_color(CTEXT_YEL);
                print_set_pos(btn->x+8, btn->y+2);
                print_dec_byte(levstars);
                print_block(BLOCK_STAR);
            }
            else {
                print_set_color(CTEXT_GRE);
                print_set_pos(btn->x+7, btn->y+2);
                print_str("SOLVED");
            }
        }
        else {
            print_set_color(CTEXT_BLU);
            print_str(levname);
            guibutton_draw(btn);
            print_set_pos(btn->x+6, btn->y+2);
            if (levsolved) print_set_color(CTEXT_GRE);
            else           print_set_color(CTEXT_YEL);
            print_str("TUTORIAL");
        }
    }

    // GUIBUTTON_INIT(sl_btn_start, SL_ST_X, SL_ST_Y, SL_ST_W, SL_ST_H);
    // guibutton_draw(&sl_btn_start);
    // print_set_pos(SL_ST_X + 3, SL_ST_Y + 2);
    // print_str("START GAME");
}

void sl_on_back()
{
    if (sg_prov_idx < 10)
        sc_switch_screen(sp_enter, sp_update, NULL);
    else
        sc_switch_screen(sb_enter, sb_update, NULL);
}

ubyte sl_over_level_idx = 0;

void sl_on_level_btn()
{
    sg_level_idx = sl_over_level_idx;
    sc_switch_level(sg_level_idx);
}

void p0_sl_update()
{
    mouseAction = nullMouseAction;

    if (guibutton_mouseinto(&sg_back_btn)) {
        mouseAction = sl_on_back;
    }
    else for (ubyte i = 0; i < sl_levcount; i++)
    {
        GuiButton* btn = &sl_level_btns[i];
        if (guibutton_mouseinto(btn)) {
            sl_over_level_idx = sl_levidx + i;
            mouseAction = sl_on_level_btn;
            break;
        }
    }

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
//     #
//    # #    #####    ####   #    #   #####
//   #   #   #    #  #    #  #    #     #
//  #     #  #####   #    #  #    #     #
//  #######  #    #  #    #  #    #     #
//  #     #  #    #  #    #  #    #     #
//  #     #  #####    ####    ####      #
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "update_build_version.h"
#include "keyb.h"

extern GuiButton sg_back_btn;
extern void sg_init_back_btn();

void sa_print_build_version();

void p0_sa_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    sp_draw_sep_line();
    sg_init_back_btn();

    print_set_color(CTEXT_WHI);
    print_set_pos(9, 1);
    print_str("Acerca de Trainyard Express");

    print_set_color(CTEXT_WHI);
    print_set_pos(1, 8);
    print_str("Version Spectrum Next por David Crespo");
    print_set_pos(5, 9);
    print_str("Desarrollada en verano de 2022");

    print_set_color(CTEXT_BLU);
    print_set_pos(9, 11);
    print_str("https://dcrespo.com");
    print_set_pos(1, 13);
    print_str("https://www.youtube.com/DavidPrograma");

    print_set_color(CTEXT_WHI);
    print_set_pos(5, 18);
    print_str("Concepto original por Matt Rix");
    print_set_pos(5, 19);
    print_str("Desarrollado para smartphones");
    print_set_pos(10, 20);
    print_str("y publicado en 2010");

    print_set_color(CTEXT_BLU);
    print_set_pos(9, 22);
    print_str("https://trainyard.ca");

    if (pd_debug_on)
        sa_print_build_version();
}

void sa_print_build_version()
{
    print_set_color(CTEXT_WHI);
    print_set_pos(4, 30);
    print_str(BUILD_STR);
}

void sa_on_back_btn()
{
    sc_switch_screen(sm_enter, sm_update, NULL);
}

void p0_sa_update()
{
    mouseAction = nullMouseAction;

    if (GBMI(&sg_back_btn)) mouseAction = sa_on_back_btn;

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    if (zxkeyQ() && zxkeyZ() && zxkeyP() && zxkeyM()) {
        pd_debug_on = 1;
        sa_print_build_version();
    }
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//   #####
//  #     #  #####    #####     #     ####   #    #   ####
//  #     #  #    #     #       #    #    #  ##   #  #
//  #     #  #    #     #       #    #    #  # #  #   ####
//  #     #  #####      #       #    #    #  #  # #       #
//  #     #  #          #       #    #    #  #   ##  #    #
//   #####   #          #       #     ####   #    #   ####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern GuiButton sg_back_btn;
extern void sg_init_back_btn();

#define SENS_BTN_CNT 3
GuiButton   so_sens_btns[SENS_BTN_CNT];
const char* so_sens_txts[SENS_BTN_CNT] = {"BAJA", "MEDIA", "ALTA" };
ubyte       so_sens_w   [SENS_BTN_CNT] = {  6,       7,      6    };

void so_draw_sens_btns();

void p0_so_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    sp_draw_sep_line();
    sg_init_back_btn();

    print_set_color(CTEXT_WHI);
    print_set_pos(9, 1);
    print_str("Opciones");

    print_set_pos(10, 6);
    print_str("Velocidad del cursor");

    so_draw_sens_btns();
}

void so_draw_sens_btns()
{
    ubyte x = 10;
    ubyte y = 8;

    for (ubyte i = 0; i < SENS_BTN_CNT; i++) {
        ubyte w = so_sens_w[i];
        GUIBUTTON_INIT(so_sens_btns[i], x, y, w, 3);
        if (i == pd_sens) print_set_color(CTEXT_BLU);
        else              print_set_color(CTEXT_WHI);
        guibutton_draw(&so_sens_btns[i]);
        print_set_pos(x+1,y+1);
        print_str(so_sens_txts[i]);
        x += w;
    }
}

void so_on_back_btn()
{
    sc_switch_screen(sm_enter, sm_update, NULL);
}

ubyte so_sens_idx = 0;

void so_on_sens_btn()
{
    pd_set_sens(so_sens_idx);
    so_draw_sens_btns();
}

void p0_so_update()
{
    mouseAction = nullMouseAction;

    if (GBMI(&sg_back_btn)) mouseAction = so_on_back_btn;
    else for (ubyte i = 0; i < SENS_BTN_CNT; i++) {
        if (GBMI(&so_sens_btns[i])) {
            so_sens_idx = i;
            mouseAction = so_on_sens_btn;
            break;
        }
    }

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
//   ###
//    #     #    #   #####  #####    ####
//    #     ##   #     #    #    #  #    #
//    #     # #  #     #    #    #  #    #
//    #     #  # #     #    #####   #    #
//    #     #   ##     #    #   #   #    #
//   ###    #    #     #    #    #   ####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern GuiButton sg_back_btn;
extern void sg_init_back_btn();

GuiButton si_btn_cont;

#define SI_CX 14
#define SI_CY 23
#define SI_CW 12
#define SI_CH 3

ubyte si_train0_idx;

void p0_si_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    si_init_trains();

    // sp_draw_sep_line();
    // sg_init_back_btn();

    print_set_color(CTEXT_WHI);
    print_set_pos(4, 7);
    print_str("Bienvenido a ");

    print_set_color(CTEXT_GRE);
    print_str("Trainyard Express!");

    print_set_color(CTEXT_WHI);
    print_set_pos(2, 10);
    print_str("En este juego se utiliza el ");
    print_set_color(CTEXT_GRE);
    print_str("cursor\n");
    print_set_color(CTEXT_WHI);
    print_str("para dibujar vias para los trenes.\n");
    print_str("\n");
    print_str("Para mover el cursor, se puede usar:\n");
    print_str("- ");
    print_set_color(CTEXT_BLU);
    print_str("Raton");
    print_set_color(CTEXT_WHI);
    print_str(", en el puerto PS/2\n");
    print_str("- ");
    print_set_color(CTEXT_BLU);
    print_str("Joystick");
    print_set_color(CTEXT_WHI);
    print_str(", en el puerto Kempston 1\n");
    print_str("- ");
    print_set_color(CTEXT_BLU);
    print_str("Teclado");
    print_set_color(CTEXT_WHI);
    print_str(", usando las teclas:\n");
    print_str("\n");
    print_set_color(CTEXT_GRE);
    print_str("QAOP/flechas .......... mover cursor\n");
    print_set_color(CTEXT_YEL);
    print_str("Espacio ......... dibujar/click izdo\n");
    print_set_color(CTEXT_ORA);
    print_str("0/borrar ......... borrar/click dcho\n");

    print_set_color(CTEXT_WHI);
    GUIBUTTON_INIT(si_btn_cont, SI_CX, SI_CY, SI_CW, SI_CH);
    guibutton_draw(&si_btn_cont);
    print_set_pos(SI_CX+2, SI_CY+1);
    print_str(" SEGUIR ");
}

void si_on_btn_cont()
{
    sc_switch_screen(sm_enter, sm_update, NULL);
}

extern void setup_2x1_sprite_ex();
#define SI_NUM_TRAINS 10
sword si_train_x[SI_NUM_TRAINS] = {
    0, 80, 160, 240, 320, 16, 96, 176, 256, 336
};
ubyte si_train_y[SI_NUM_TRAINS] = {
    8, 8, 8, 8, 8, 232, 232, 232, 232, 232
};
ubyte si_train_c[SI_NUM_TRAINS] = {
    16 * COLOR_RED, 16 * COLOR_YEL, 16 * COLOR_GRE, 16 * COLOR_BLU,
    16 * COLOR_RED,
    16 * COLOR_RED + ATTR_X___,
    16 * COLOR_BLU + ATTR_X___, 16 * COLOR_GRE + ATTR_X___, 
    16 * COLOR_YEL + ATTR_X___, 16 * COLOR_RED + ATTR_X___
};

ubyte si_last_train_idx_A = 4;
ubyte si_last_train_idx_B = 5;

void si_update_train(ubyte idx)
{
    param0b = si_train0_idx + idx * 2;
    param1b = 0;
    param2w = si_train_x[idx];
    param3b = si_train_y[idx];
    param4b = si_train_c[idx];
    param5b = 16;
    param6b = 0;

    setup_2x1_sprite_ex();

    sbyte i0, i1;
    sword x = param2w;
    if (idx < 5) {
        x++;
        if (x > 360) {
            x = -40;
            i0 = si_last_train_idx_A;
            i1 = i0 - 1;
            if (i1 < 0) i1 = 4;
            si_train_c[i0] = si_train_c[i1];
            si_last_train_idx_A = i1;
        }
    }
    else {
        x--;
        if (x < -40) {
            x = 360;
            i0 = si_last_train_idx_B;
            i1 = i0 + 1;
            if (i1 > 9) i1 = 5;
            si_train_c[i0] = si_train_c[i1];
            si_last_train_idx_B = i1;
        }
    }
    si_train_x[idx] = x;

    // param0b = si_train0_idx + idx + 2;
    // param1b = 0;
    // param2b = 32;
    // param3b = 232;
    // param4b = (COLOR_BLU << 4) | ATTR_X___;
    // param5b = 16;
    // param6b = 0;

    // setup_2x1_sprite_ex();
}

void si_init_trains()
{
    #define UNFRAMED_TRACK_WE 54
    for (ubyte x = 0; x < 40; x += 4) {
        draw_tile_xy(x,  0, UNFRAMED_TRACK_WE);
        draw_tile_xy(x, 28, UNFRAMED_TRACK_WE);
    }

    si_train0_idx = spmgr_curr_idx;
}

void si_update_trains()
{
    for (ubyte i = 0; i < SI_NUM_TRAINS; i++) {
        si_update_train(i);
    }
}

void p0_si_update()
{
    mouseAction = nullMouseAction;

    if (GBMI(&si_btn_cont)) mouseAction = si_on_btn_cont;

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    // print_set_pos(0, 0);
    // print_dec_word(mouse_x);
    // print_char(' ');
    // print_dec_byte(mouse_y);

    si_update_trains();
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######
//  #        #####      #    #        ####    ####
//  #        #    #     #    #       #    #  #    #
//  #####    #    #     #    #       #    #  #
//  #        #####      #    #       #    #  #  ###
//  #        #          #    #       #    #  #    #
//  #######  #          #    ######   ####    ####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "screen_epilog.h"
#include "gui.h"
#include "spmgr.h"
#include "mouse.h"
#include "colors.h"
#include "screen_controller.h"
#include "screen_provs.h"
#include "screen_game.h"
#include "print_tile.h"
#include "level.h"
#include "sound_freqs.h"

#define SE_FR_X 6
#define SE_FR_Y 8
#define SE_FR_W 28
#define SE_FR_H 16

#define SE_ST_X 8
#define SE_ST_Y 12
#define SE_ST_W 24
#define SE_ST_H 4

#define SE_RP_X 9
#define SE_RP_Y 17
#define SE_RP_W 11
#define SE_RP_H 5

#define SE_NE_X 20
#define SE_NE_Y 17
#define SE_NE_W 11
#define SE_NE_H 5

GuiButton se_btn_replay;
GuiButton se_btn_next;

uword se_ts;

void p0_se_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    print_set_color(CTEXT_WHI);
    print_set_symbol(BLOCK_OUTER_BORDER);
    print_frame(SE_FR_X, SE_FR_Y, SE_FR_W, SE_FR_H);

    print_set_color(CTEXT_GRE);

    print_set_pos(SE_FR_X + 7, SE_FR_Y + 2);
    // print_str("YOU SOLVED IT!");
    print_str("LO HAS RESUELTO!");

    if (sg_score_earned) {
        print_set_color(CTEXT_WHI);
        print_set_symbol(BLOCK_FINE_BORDER);
        print_frame(SE_ST_X, SE_ST_Y, SE_ST_W, SE_ST_H);

        print_set_pos(SE_ST_X + 2, SE_ST_Y + 1);
        // print_str("STARS EARNED:");
        print_str("ESTRELLAS GANADAS: ");
        // print_set_pos(SE_ST_X + 7, SE_ST_Y + 2);
        print_set_color(CTEXT_GRE);
        print_dec_byte(lev_stars);
        print_block(BLOCK_STAR);
    }

    print_set_color(CTEXT_BLU);
    GUIBUTTON_INIT(se_btn_replay, SE_RP_X, SE_RP_Y, SE_RP_W, SE_RP_H);
    guibutton_draw(&se_btn_replay);

    print_set_color(CTEXT_WHI);
    print_set_pos(SE_RP_X + 2, SE_RP_Y + 2);
    print_str("REPETIR");

    print_set_color(CTEXT_YEL);
    GUIBUTTON_INIT(se_btn_next, SE_NE_X, SE_NE_Y, SE_NE_W, SE_NE_H);
    guibutton_draw(&se_btn_next);

    print_set_color(CTEXT_WHI);
    print_set_pos(SE_NE_X + 1, SE_NE_Y + 2);

    if (sc_is_level_unlocked(sg_level_idx+1)) {
        print_str("SIGUIENTE");
    }
    else {
        print_str("  MENU  ");

        if (pd_score < STARS_IN_BONUS_LEVELS) {
            print_set_pos(0, SE_FR_Y + SE_FR_H + 2);
            println_ctr("Siguiente nivel bloqueado,", 40);
            println_ctr("desbloquea niveles previos", 40);
        }
    }

    se_ts = 0;
}

void se_on_btn_replay()
{
    sc_switch_level(sg_level_idx);
}

void se_on_btn_next()
{
    sc_switch_next_level();
}

void p0_se_update()
{
    mouseAction = nullMouseAction;

    if      (guibutton_mouseinto(&se_btn_replay)) mouseAction = se_on_btn_replay;
    else if (guibutton_mouseinto(&se_btn_next)  ) mouseAction = se_on_btn_next;

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    #define SE_DT 10
    se_ts++;

    if (se_ts ==  1 * SE_DT) { snd_play_tone(T_C5); return; }
    if (se_ts ==  2 * SE_DT) { snd_play_tone(T_E5); return; }
    if (se_ts ==  3 * SE_DT) { snd_play_tone(T_G5); return; }
    if (se_ts ==  4 * SE_DT) { snd_play_tone(T_B5); return; }
    if (se_ts ==  5 * SE_DT) { snd_play_tone(T_D5); return; }
    if (se_ts ==  6 * SE_DT) { snd_play_tone(T_F5); return; }
    if (se_ts ==  7 * SE_DT) { snd_play_tone(T_A5); return; }
    if (se_ts ==  8 * SE_DT) { snd_play_tone(T_C6); return; }
}




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 
//  #     #
//  #     #     #     ####    #####   ####   #####    #   #
//  #     #     #    #    #     #    #    #  #    #    # #
//  #     #     #    #          #    #    #  #    #     #
//   #   #      #    #          #    #    #  #####      #
//    # #       #    #    #     #    #    #  #   #      #
//     #        #     ####      #     ####   #    #     #
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "screen_epilog.h"
#include "gui.h"
#include "spmgr.h"
#include "mouse.h"
#include "colors.h"
#include "screen_controller.h"
#include "screen_provs.h"
#include "screen_game.h"
#include "print_tile.h"
#include "level.h"

#define SV_FR_X 6
#define SV_FR_Y 7
#define SV_FR_W 28
#define SV_FR_H 19

#define SV_ST_X 12
#define SV_ST_Y 12
#define SV_ST_W 16
#define SV_ST_H 4

#define SV_RP_X 12
#define SV_RP_Y 19
#define SV_RP_W 16
#define SV_RP_H 5

uword sv_ts;

GuiButton sv_btn_replay;
GuiButton sv_btn_next;

void p0_sv_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    print_set_color(CTEXT_WHI);
    print_set_symbol(BLOCK_OUTER_BORDER);
    print_frame(SV_FR_X, SV_FR_Y, SV_FR_W, SV_FR_H);

    print_set_pos(SV_FR_X + 1, SV_FR_Y + 2);

    print_set_color(CTEXT_WHI);
    println_ctr("Felicidades,", SV_FR_W-2);
    println("");
    print_set_color(CTEXT_YEL);
    println_ctr("\x1f HAS GANADO EL JUEGO! \x1f", SV_FR_W-2);
    println("");
    print_set_color(CTEXT_WHI);
    println_ctr("(bien hecho, no es facil)", SV_FR_W-2);

    println("");
    //           0123456789012345678901234567
    println_ctr("Ahora puedes jugar los"    , SV_FR_W-2);
    println_ctr("puzzles extra, desde", SV_FR_W-2);
    println_ctr("el menu principal"      , SV_FR_W-2);

    print_set_color(CTEXT_BLU);
    GUIBUTTON_INIT(sv_btn_replay, SV_RP_X, SV_RP_Y, SV_RP_W, SV_RP_H);
    guibutton_draw(&sv_btn_replay);

    print_set_color(CTEXT_WHI);
    print_set_pos(SV_RP_X + 3, SV_RP_Y + 2);
    print_str("IR AL MENU");

    sv_ts = 0;
}

void sv_on_btn_replay()
{
    sc_switch_screen(sm_enter, sm_update, NULL);
}

void p0_sv_update()
{
    mouseAction = nullMouseAction;

    if      (guibutton_mouseinto(&sv_btn_replay)) mouseAction = sv_on_btn_replay;

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    #define SV_DT 12
    sv_ts++;

    if (sv_ts ==  1 * SV_DT) { snd_play_tone(T_C5); return; }
    if (sv_ts ==  2 * SV_DT) { snd_play_tone(T_D5); return; }
    if (sv_ts ==  3 * SV_DT) { snd_play_tone(T_E5); return; }
    if (sv_ts ==  4 * SV_DT) { snd_play_tone(T_F5); return; }
    if (sv_ts ==  5 * SV_DT) { snd_play_tone(T_G5); return; }
    if (sv_ts ==  6 * SV_DT) { snd_play_tone(T_E5); return; }
    if (sv_ts ==  7 * SV_DT) { snd_play_tone(T_G5); return; }
    if (sv_ts ==  8 * SV_DT) { snd_play_tone(T_C6); return; }
    if (sv_ts ==  9 * SV_DT) { snd_play_tone(T_G5); return; }
    if (sv_ts == 10 * SV_DT) { snd_play_tone(T_E5); return; }
    if (sv_ts == 11 * SV_DT) { snd_play_tone(T_C5); return; }
    if (sv_ts == 12 * SV_DT) { snd_play_tone(T_E5); return; }
    if (sv_ts == 13 * SV_DT) { snd_play_tone(T_G5); return; }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 
//  #     #
//  #     #     #     ####    #####   ####   #####    #   #
//  #     #     #    #    #     #    #    #  #    #    # #
//  #     #     #    #          #    #    #  #    #     #
//  #     #     #    #          #    #    #  #####      #
//  #  #  #     #    #    #     #    #    #  #   #      #
//   ## ##      #     ####      #     ####   #    #     #
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "screen_epilog.h"
#include "gui.h"
#include "spmgr.h"
#include "mouse.h"
#include "colors.h"
#include "screen_controller.h"
#include "screen_provs.h"
#include "screen_game.h"
#include "print_tile.h"
#include "level.h"

#define SW_FR_X 2
#define SW_FR_Y 7
#define SW_FR_W 36
#define SW_FR_H 19

#define SW_ST_X 12
#define SW_ST_Y 12
#define SW_ST_W 16
#define SW_ST_H 4

#define SW_RP_X 12
#define SW_RP_Y 19
#define SW_RP_W 16
#define SW_RP_H 5

uword sw_ts;

GuiButton sw_btn_replay;
GuiButton sw_btn_next;

void p0_sw_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    print_set_color(CTEXT_WHI);
    print_set_symbol(BLOCK_OUTER_BORDER);
    print_frame(SW_FR_X, SW_FR_Y, SW_FR_W, SW_FR_H);

    print_set_pos(SW_FR_X + 1, SW_FR_Y + 2);

    print_set_color(CTEXT_WHI);
    println_ctr("Felicidades,", SW_FR_W-2);
    println("");
    print_set_color(CTEXT_YEL);
    println_ctr("\x1f\x1f SUPERASTE LOS PUZZLES EXTRA! \x1f\x1f", SW_FR_W-2);
    println("");
    print_set_color(CTEXT_WHI);

    println("");
    println("");
    //           0123456789012345678901234567
    println_ctr("ERES GENIAL."    , SW_FR_W-2);

    print_set_color(CTEXT_BLU);
    GUIBUTTON_INIT(sw_btn_replay, SW_RP_X, SW_RP_Y, SW_RP_W, SW_RP_H);
    guibutton_draw(&sw_btn_replay);

    print_set_color(CTEXT_WHI);
    print_set_pos(SW_RP_X + 3, SW_RP_Y + 2);
    print_str("IR AL MENU");

    sw_ts = 0;
}

void sw_on_btn_replay()
{
    sc_switch_screen(sm_enter, sm_update, NULL);
}

void p0_sw_update()
{
    mouseAction = nullMouseAction;

    if      (guibutton_mouseinto(&sw_btn_replay)) mouseAction = sw_on_btn_replay;

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    #define SW_DT 12
    sw_ts++;

    if (sw_ts ==  1 * SW_DT) { snd_play_tone(T_C5); return; }
    if (sw_ts ==  2 * SW_DT) { snd_play_tone(T_D5); return; }
    if (sw_ts ==  3 * SW_DT) { snd_play_tone(T_E5); return; }
    if (sw_ts ==  4 * SW_DT) { snd_play_tone(T_F5); return; }
    if (sw_ts ==  5 * SW_DT) { snd_play_tone(T_G5); return; }
    if (sw_ts ==  6 * SW_DT) { snd_play_tone(T_E5); return; }
    if (sw_ts ==  7 * SW_DT) { snd_play_tone(T_G5); return; }
    if (sw_ts ==  8 * SW_DT) { snd_play_tone(T_C6); return; }
    if (sw_ts ==  9 * SW_DT) { snd_play_tone(T_G5); return; }
    if (sw_ts == 10 * SW_DT) { snd_play_tone(T_A5); return; }
    if (sw_ts == 11 * SW_DT) { snd_play_tone(T_Bb5); return; }
    if (sw_ts == 12 * SW_DT) { snd_play_tone(T_B5); return; }
    if (sw_ts == 13 * SW_DT) { snd_play_tone(T_C6); return; }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######
//     #     ######   ####    #####   ####
//     #     #       #          #    #
//     #     #####    ####      #     ####
//     #     #            #     #         #
//     #     #       #    #     #    #    #
//     #     ######   ####      #     ####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "screen_tests.h"
#include "sound.h"

void p0_st_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    sp_draw_sep_line();
    sg_init_back_btn();

    print_set_color(CTEXT_WHI);
    print_set_pos(9, 1);
    print_str("Tests");

    // ...

}

void st_on_back_btn()
{
    sc_switch_screen(sm_enter, sm_update, NULL);
}

void p0_st_update()
{
    mouseAction = nullMouseAction;

    if (GBMI(&sg_back_btn)) mouseAction = st_on_back_btn;

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    // ...

}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######            #
//     #              # #    #    #  #####      #     ####
//     #             #   #   #    #  #    #     #    #    #
//     #     #####  #     #  #    #  #    #     #    #    #
//     #            #######  #    #  #    #     #    #    #
//     #            #     #  #    #  #    #     #    #    #
//     #            #     #   ####   #####      #     ####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "sound_freqs.h"
#include "sound.h"

#define STA_NCNT 8
GuiButton sta_note_btns[STA_NCNT];
uword     sta_note_vals[STA_NCNT] = { T_C5, T_D5, T_E5, T_F5, T_G5, T_A5, T_B5, T_C6 };
char      sta_note_chrs[STA_NCNT] = {  'C',  'D',  'E',  'F',  'G',  'A',  'B',  'C' };

GuiButton sta_panic_btn;
GuiButton sta_crash_btn;
GuiButton sta_switch_btn;
GuiButton sta_success_btn;

void p0_sta_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    sp_draw_sep_line();
    sg_init_back_btn();

    print_set_color(CTEXT_WHI);
    print_set_pos(9, 1);
    print_str("Audio test");

    // ...

    ubyte x = 2;
    ubyte y = 4;

    for (ubyte i = 0; i < STA_NCNT; i++) {
        GUIBUTTON_INIT(sta_note_btns[i], x, y, 5, 3);
        guibutton_draw(&sta_note_btns[i]);
        print_set_pos(x + 2, y + 1);
        print_char(sta_note_chrs[i]);
        y += 3;
    }

    GUIBUTTON_INIT(sta_panic_btn, x, y, 7, 3);
    guibutton_draw(&sta_panic_btn);
    print_set_pos(x + 1, y + 1);
    print_str("PANIC");
    y += 3;

    x = 8;
    y = 4;

    GUIBUTTON_INIT(sta_crash_btn, x, y, 9, 3);
    guibutton_draw(&sta_crash_btn);
    print_set_pos(x + 1, y + 1);
    print_str(" Crash");
    y += 3;

    GUIBUTTON_INIT(sta_switch_btn, x, y, 9, 3);
    guibutton_draw(&sta_switch_btn);
    print_set_pos(x + 1, y + 1);
    print_str("Switch");
    y += 3;

    GUIBUTTON_INIT(sta_success_btn, x, y, 9, 3);
    guibutton_draw(&sta_success_btn);
    print_set_pos(x + 1, y + 1);
    print_str("Success");
    y += 3;

}

void sta_on_back_btn()
{
    sc_switch_screen(st_enter, st_update, NULL);
}

ubyte sta_tone_idx;

void sta_on_tone_btn()
{
    snd_play_tone(sta_note_vals[sta_tone_idx]);
}

void sta_on_panic_btn()
{
    snd_panic();
}

void sta_on_crash_btn()
{
    snd_play_crash();
}

void sta_on_switch_btn()
{
    snd_play_switch();
}

void sta_on_success_btn()
{
    snd_play_success();
}

void p0_sta_update()
{
    mouseAction = nullMouseAction;

    if      (GBMI(  &sg_back_btn)) mouseAction = sta_on_back_btn;
    else if (GBMI(&sta_panic_btn)) mouseAction = sta_on_panic_btn;
    else if (GBMI(&sta_crash_btn)) mouseAction = sta_on_crash_btn;
    else if (GBMI(&sta_switch_btn)) mouseAction = sta_on_switch_btn;
    else if (GBMI(&sta_success_btn)) mouseAction = sta_on_success_btn;
    else for (ubyte i = 0; i < STA_NCNT; i++) {
        if (GBMI(&sta_note_btns[i])) {
            mouseAction = sta_on_tone_btn;
            sta_tone_idx = i;
            break;
        }
    }

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    // ...

    print_set_color(CTEXT_WHI);
    print_set_pos(20, 5);
    for (ubyte i = 0; i < 11; i++) {
        print_hex_nibble(i);
        print_char(' ');
        snd_select_ay(1);
        print_hex_byte(snd_get_ay_reg8(i));
        print_char(' ');
        snd_select_ay(2);
        print_hex_byte(snd_get_ay_reg8(i));
        print_char(' ');
        snd_select_ay(3);
        print_hex_byte(snd_get_ay_reg8(i));
        print_char('\n');
    }

}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######         #    #
//     #            #   #    ######   #   #  #####    ####     ##    #####   #####
//     #            #  #     #         # #   #    #  #    #   #  #   #    #  #    #
//     #     #####  ###      #####      #    #####   #    #  #    #  #    #  #    #
//     #            #  #     #          #    #    #  #    #  ######  #####   #    #
//     #            #   #    #          #    #    #  #    #  #    #  #   #   #    #
//     #            #    #   ######     #    #####    ####   #    #  #    #  #####
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p0_stk_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    sp_draw_sep_line();
    sg_init_back_btn();

    print_set_color(CTEXT_WHI);
    print_set_pos(9, 1);
    print_str("Keyboard test");

    // ...
}

void stk_on_back_btn()
{
    sc_switch_screen(st_enter, st_update, NULL);
}

void p0_stk_update()
{
    mouseAction = nullMouseAction;

    if (GBMI(&sg_back_btn)) mouseAction = stk_on_back_btn;

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    // ...

}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  #######          #####
//     #            #     #   #   #   ####    #####  ######  #    #
//     #            #          # #   #          #    #       ##  ##
//     #     #####   #####      #     ####      #    #####   # ## #
//     #                  #     #         #     #    #       #    #
//     #            #     #     #    #    #     #    #       #    #
//     #             #####      #     ####      #    ######  #    #
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p0_sts_enter()
{
    spmgr_init();
    spmgr_setup_cursor();

    gui_clear_screen();

    sp_draw_sep_line();
    sg_init_back_btn();

    print_set_color(CTEXT_WHI);
    print_set_pos(9, 1);
    print_str("System test");

    // ...

}

void sts_on_back_btn()
{
    sc_switch_screen(st_enter, st_update, NULL);
}

void p0_sts_update()
{
    mouseAction = nullMouseAction;

    if (GBMI(&sg_back_btn)) mouseAction = sts_on_back_btn;

    if (mouseAction == nullMouseAction) spmgr_cursor_set_arrow();
    else                                spmgr_cursor_set_hand();

    spmgr_update_cursor(mouse_x, mouse_y);

    // check clicked buttons
    if (mouse_released_l) mouseAction();

    // ...

}



