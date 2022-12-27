#include "mouse.h"
#include "print_tile.h"
#include "spmgr.h"
#include "keyb.h"
#include "joystick.h"
#include <z80.h>

#define INITIAL_MOUSE_POS_X 290
#define INITIAL_MOUSE_POS_Y 88

#define MOUSE_L_POS 0x02
#define MOUSE_R_POS 0x01
#define MOUSE_L_NEG 0xFD
#define MOUSE_R_NEG 0xFE


ubyte mouse_raw_x;
ubyte mouse_raw_y;
ubyte mouse_raw_b;
ubyte mouse_key_b;
ubyte mouse_joy_b;

ubyte mouse_prw_x;
ubyte mouse_prw_y;
ubyte mouse_prw_b;
ubyte mouse_pr2_b;

ubyte mouse_dif_x;
ubyte mouse_dif_y;

uword mouse_x;
ubyte mouse_y;

ubyte mouse_hx;
ubyte mouse_hy;

ubyte mouse_but_l;
ubyte mouse_but_r;

ubyte mouse_prb_l;
ubyte mouse_prb_r;

ubyte mouse_pressed_l;
ubyte mouse_pressed_r;

ubyte mouse_released_l;
ubyte mouse_released_r;

ubyte mouse_dblclick_l;
ubyte mouse_dblclick_r;

#define MOUSE_DBLCLICK_FRAMES 16

sword mouse_tmp_x;
sword mouse_tmp_y;

uword mouse_ts = 0;
uword mouse_ts_clk_l = 0;
uword mouse_ts_clk_r = 0;

void mouse_init()
{
    mouse_raw_x = z80_inp(0xFBDF);
    mouse_raw_y = z80_inp(0xFFDF);
    mouse_raw_b = z80_inp(0xFADF);
    mouse_key_b = 0;

    mouse_prw_x = mouse_raw_x;
    mouse_prw_y = mouse_raw_y;

    mouse_pressed_l = mouse_released_l = 0;
    mouse_pressed_r = mouse_released_r = 0;

    mouse_x = mouse_tmp_x = INITIAL_MOUSE_POS_X;
    mouse_y = mouse_tmp_y = INITIAL_MOUSE_POS_Y;

    mouse_hx = mouse_x >> 1;
    mouse_hy = mouse_y >> 1;

    mouse_but_l = mouse_prb_l = 0;
    mouse_but_r = mouse_prb_r = 0;

    spmgr_update_cursor(mouse_x, mouse_y);
}

void mouse_apply_x()
{
    if (mouse_tmp_x < 0)
        mouse_tmp_x = 0;
    if (mouse_tmp_x > 319)
        mouse_tmp_x = 319;

    mouse_x = mouse_tmp_x;
    mouse_hx = mouse_x >> 1;
}

void mouse_apply_y()
{
    if (mouse_tmp_y < 0)
        mouse_tmp_y = 0;
    if (mouse_tmp_y > 255)
        mouse_tmp_y = 255;

    mouse_y = mouse_tmp_y;
    mouse_hy = mouse_y >> 1;
}

ubyte keyjoy_delta = 2;

void mouse_update_joy()
{
    // ubyte delta = 2;
    // if (zxkeyCAP()) delta <<= 1;
    // if (zxkeySYM()) delta >>= 1;

    if (joyUp)
    {
        mouse_tmp_y -= keyjoy_delta;
        mouse_apply_y();
    }
    if (joyDown)
    {
        mouse_tmp_y += keyjoy_delta;
        mouse_apply_y();
    }
    if (joyLeft)
    {
        mouse_tmp_x -= keyjoy_delta;
        mouse_apply_x();
    }
    if (joyRight)
    {
        mouse_tmp_x += keyjoy_delta;
        mouse_apply_x();
    }

    mouse_joy_b = 0xFF;
    if (joyBut1) {
        mouse_joy_b &= MOUSE_L_NEG;
        // print_set_pos(39,0);
        // print_char('_');
    }

    if (joyBut2) {
        mouse_joy_b &= MOUSE_R_NEG;
        // print_set_pos(39,0);
        // print_char('0');
    }
}

void mouse_update_key()
{
    // ubyte delta = 2;
    // if (zxkeyCAP()) delta <<= 1;
    // if (zxkeySYM()) delta >>= 1;

    if (keyUp)
    {
        mouse_tmp_y -= keyjoy_delta;
        mouse_apply_y();
    }
    if (keyDown)
    {
        mouse_tmp_y += keyjoy_delta;
        mouse_apply_y();
    }
    if (keyLeft)
    {
        mouse_tmp_x -= keyjoy_delta;
        mouse_apply_x();
    }
    if (keyRight)
    {
        mouse_tmp_x += keyjoy_delta;
        mouse_apply_x();
    }

    mouse_key_b = 0xFF;
    if (keyBut1) {
        mouse_key_b &= MOUSE_L_NEG;
        // print_set_pos(39,0);
        // print_char('_');
    }

    if (keyBut2) {
        mouse_key_b &= MOUSE_R_NEG;
        // print_set_pos(39,0);
        // print_char('0');
    }
}

void mouse_update_pos()
{
    mouse_raw_x = z80_inp(0xFBDF);
    mouse_raw_y = z80_inp(0xFFDF);

    if (mouse_raw_x == mouse_prw_x && mouse_raw_y == mouse_prw_y) return;

    mouse_dif_x = mouse_raw_x - mouse_prw_x;
    mouse_dif_y = mouse_prw_y - mouse_raw_y;    // Y axis inversion

    mouse_prw_x = mouse_raw_x;
    mouse_prw_y = mouse_raw_y;

    mouse_tmp_x += mouse_dif_x;
    if (mouse_dif_x & 0x80)
        mouse_tmp_x -= 0x100;

    mouse_apply_x();

    mouse_tmp_y += mouse_dif_y;
    if (mouse_dif_y & 0x80)
        mouse_tmp_y -= 0x100;

    mouse_apply_y();

    // spmgr_update_cursor(mouse_x, mouse_y);
}

void mouse_update_btn()
{
    mouse_raw_b = z80_inp(0xFADF);
    mouse_raw_b &= mouse_key_b;
    mouse_raw_b &= mouse_joy_b;
    // print_set_pos(38,0);
    // print_hex_byte(mouse_raw_b);
    // print_set_pos(38,1);
    // print_hex_byte(mouse_key_b);

    if (mouse_raw_b == mouse_prw_b && mouse_raw_b == mouse_pr2_b) return;

    mouse_but_l = mouse_but_r = 1;
    if (mouse_raw_b & MOUSE_L_POS) mouse_but_l = 0;
    if (mouse_raw_b & MOUSE_R_POS) mouse_but_r = 0;

    mouse_prb_l = mouse_prb_r = 1;
    if (mouse_prw_b & MOUSE_L_POS) mouse_prb_l = 0;
    if (mouse_prw_b & MOUSE_R_POS) mouse_prb_r = 0;

    mouse_pressed_l = 0;
    mouse_released_l = 0;
    mouse_dblclick_l = 0;
    if (mouse_but_l != mouse_prb_l) {
        mouse_pressed_l = mouse_but_l;
        mouse_released_l = mouse_prb_l;
        if (mouse_released_l) {
            if (mouse_ts - mouse_ts_clk_l <= MOUSE_DBLCLICK_FRAMES)
                mouse_dblclick_l = 1;
            else
                mouse_ts_clk_l = mouse_ts;
        }
    }

    mouse_pressed_r = 0;
    mouse_released_r = 0;
    mouse_dblclick_r = 0;
    if (mouse_but_r != mouse_prb_r) {
        mouse_pressed_r = mouse_but_r;
        mouse_released_r = mouse_prb_r;
        if (mouse_released_r) {
            if (mouse_ts - mouse_ts_clk_r <= MOUSE_DBLCLICK_FRAMES)
                mouse_dblclick_r = 1;
            else
                mouse_ts_clk_r = mouse_ts;
        }
    }

    mouse_pr2_b = mouse_prw_b;
    mouse_prw_b = mouse_raw_b;
}

void mouse_update()
{
    mouse_update_joy();
    mouse_update_key();
    mouse_update_pos();
    mouse_update_btn();
    mouse_ts++;
}

MouseAction mouseAction = nullMouseAction;

void nullMouseAction()
{
    // nothing to see here, move along
}

void mouse_print_debug()
{
    // ubyte x = 30;
    // ubyte y = 2;

    // print_set_pos(x, y);
    // print_str("MOUSE INFO");
    // y += 2;

    // print_set_pos(x, y);
    // print_str("POSITION");
    // y += 2;

    // x++;
    // print_set_pos(x, y++);
    // print_str("RAWX ");
    // print_hex_byte(mouse_raw_x);
    // print_set_pos(x, y++);
    // print_str("RAWY ");
    // print_hex_byte(mouse_raw_y);
    // print_set_pos(x, y++);
    // print_str("DIFX ");
    // print_hex_byte(mouse_dif_x);
    // print_set_pos(x, y++);
    // print_str("DIFY ");
    // print_hex_byte(mouse_dif_y);
    // print_set_pos(x, y++);
    // print_str("MX ");
    // print_hex_word(mouse_x);
    // print_set_pos(x, y++);
    // print_str("MY ");
    // print_hex_word(mouse_y);
    // print_set_pos(x, y++);
    // print_str("HX   ");
    // print_hex_byte(mouse_hx);
    // print_set_pos(x, y++);
    // print_str("HY   ");
    // print_hex_byte(mouse_hy);
    // x--;
    // y++;

    // print_set_pos(x, y);
    // print_str("BUTTONS");
    // y += 2;

    // x++;
    // print_set_pos(x, y++);
    // print_str("RAW ");
    // print_hex_byte(mouse_raw_b);

    // print_set_pos(x, y++);
    // print_str("BTN ");
    // if (mouse_but_l) {
    //     if (mouse_but_r) print_str("LR");
    //     else             print_str("L ");
    // }
    // else if (mouse_but_r) print_str(" R");
    // else                  print_str("  ");
    
    // print_set_pos(x, y++);
    // print_str("PRE ");
    // if (mouse_pressed_l) {
    //     if (mouse_pressed_r) print_str("LR");
    //     else             print_str("L ");
    // }
    // else if (mouse_pressed_r) print_str(" R");
    // else                  print_str("  ");
    
    // print_set_pos(x, y++);
    // print_str("REL ");
    // if (mouse_released_l) {
    //     if (mouse_released_r) print_str("LR");
    //     else             print_str("L ");
    // }
    // else if (mouse_released_r) print_str(" R");
    // else                  print_str("  ");
    
    // print_set_pos(x, y++);
    // print_str("DBL ");
    // if (mouse_dblclick_l) {
    //     if (mouse_dblclick_r) print_str("LR");
    //     else             print_str("L ");
    // }
    // else if (mouse_dblclick_r) print_str(" R");
    // else                  print_str("  ");
    
    // x--;
}

