#include "design.h"
#include "spmgr.h"
#include "mouse.h"
#include "board.h"
#include "tilesymbols.h"
#include "print_tile.h"
#include "level.h"
#include "colors.h"
#include "player_data.h"

static ubyte i0 = 7;
static ubyte j0 = 7;

static ubyte pi, pj;

static ubyte nt;

static ubyte i1, j1;
static ubyte i2, j2;

static ubyte activeL = 0;
static ubyte activeR = 0;

static ubyte isFake = 0;

void design_init()
{
    spmgr_init();
    spmgr_setup_cursor();
    spmgr_setup_marker();
    spmgr_end_fixed_sprites();
    spmgr_hide_marker();

    i0 = 7;
    j0 = 7;
    activeL = activeR = 0;
}

static void deleteTile()
{
    ubyte tval = board_get_tile(i0, j0);
    if (!tile_is_fixed(tval)) {
        board_set_tile(i0, j0, XXXX);
        board_draw_tile(i0, j0, XXXX);
    }
}

static void addTile()
{
    ubyte pval = board_get_tile(i1, j1);
    if (tile_is_fixed(pval))
        return;

    ubyte dir01 = 0;
    ubyte dir12 = 0;

    if (i0 < i1) dir01 = DIRW;
    if (i1 < i0) dir01 = DIRE;
    if (j0 < j1) dir01 = DIRN;
    if (j1 < j0) dir01 = DIRS;

    if (i1 < i2) dir12 = DIRE;
    if (i2 < i1) dir12 = DIRW;
    if (j1 < j2) dir12 = DIRS;
    if (j2 < j1) dir12 = DIRN;

    ubyte tval = dir01 | dir12;
    ubyte tval4 = tval << 4;

    if (!tile_is_track(tval4)) return;

    if (pval == 0)
        tval = tval4 | tval;
    else {
        tval = pval << 4 | tval;
    }

    board_set_tile(i1, j1, tval);
    board_draw_tile(i1, j1, tval);
}

static void switchTrack()
{
    ubyte pval = board_get_tile(i0, j0);
    if (!tile_is_track(pval))
        return;

    ubyte pv0 = pval & 0xF;
    ubyte pv1 = pval >> 4;
    pval = pv1 | (pv0 << 4);
    board_set_tile(i0, j0, pval);
    board_draw_tile(i0, j0, pval);
}

static void onLClick()
{
    if (!isFake)
        pd_snapshot();

    pi = i0;
    pj = j0;
    nt = 0;
    activeL = 1;
    activeR = 0;
}

static void onLDrag()
{
    if (!activeL) return;
    if (i0 == pi && j0 == pj) return;

    nt++;

    if (nt == 1) {
        i1 = pi;
        j1 = pj;
    }
    else {
        i2 = i1;
        j2 = j1;
        i1 = pi;
        j1 = pj;
        addTile();
    }

    pi = i0;
    pj = j0;
}

static void onLRelease()
{
    activeL = 0;
    activeR = 0;
}

static void onLDblClick()
{
    if (!isFake)
        pd_snapshot();

    switchTrack();
}

static void onRClick()
{
    if (!isFake)
        pd_snapshot();

    deleteTile();
    i1 = i0;
    j1 = j0;
    activeR = 1;
    activeL = 0;
}

static void onRDrag()
{
    if (!activeR) return;
    if (i0 == i1 && j0 == j1) return;
    i1 = i0;
    j1 = j0;
    deleteTile();
}

static void onRRelease()
{
    activeR = 0;
    activeL = 0;
}

static void process_mouse()
{
    if (level_state != LEV_STATE_DESIGN) {
        i0 = 7;
        j0 = 7;
        activeL = activeR = 0;
        return;
    }

    i0 = mouse_hx;
    i0 -= 4 * BOARD_X;
    i0 >>= 4;

    j0 = mouse_hy;
    j0 -= 4 * BOARD_Y;
    j0 >>= 4;

    if (i0 >= 7 || j0 >= 7) {
        activeL = activeR = 0;
        return;
    }

    isFake = 0;
    if (mouse_pressed_l)  onLClick();
    if (mouse_but_l)      onLDrag();
    if (mouse_dblclick_l) onLDblClick();
    if (mouse_released_l) onLRelease();
    if (mouse_pressed_r)  onRClick();
    if (mouse_but_r)      onRDrag();
    if (mouse_released_r) onRRelease();
}

ubyte fake_hx;
ubyte fake_hy;
ubyte fake_pressed_l;
ubyte fake_but_l;
ubyte fake_dblclick_l;
ubyte fake_released_l;
ubyte fake_pressed_r;
ubyte fake_but_r;
ubyte fake_released_r;

static void process_fake()
{
    if (level_state != LEV_STATE_DESIGN) {
        i0 = 7;
        j0 = 7;
        activeL = activeR = 0;
        return;
    }

    i0 = fake_hx;
    i0 -= 4 * BOARD_X;
    i0 >>= 4;

    j0 = fake_hy;
    j0 -= 4 * BOARD_Y;
    j0 >>= 4;

    if (i0 >= 7 || j0 >= 7) {
        activeL = activeR = 0;
        return;
    }

    isFake = 1;
    if (fake_pressed_l)  onLClick();
    if (fake_but_l)      onLDrag();
    if (fake_dblclick_l) onLDblClick();
    if (fake_released_l) onLRelease();
    if (fake_pressed_r)  onRClick();
    if (fake_but_r)      onRDrag();
    if (fake_released_r) onRRelease();
}

void update_marker()
{
    if (i0 >= 7 || j0 >= 7) {
        spmgr_hide_marker();
        return;
    }

    spmgr_show_marker();
    ubyte x = (BOARD_X + i0 * 4) * 8;
    ubyte y = (BOARD_Y + j0 * 4) * 8;
    spmgr_pos_marker(x, y);
}

void design_update()
{
    process_mouse();
    update_marker();
}

void design_update_fake()
{
    process_fake();
    update_marker();
}

void design_print_debug()
{
    // ubyte x = 30;
    // ubyte y = 2;
    // print_set_color(CTEXT_WHI);

    // print_set_pos(x, y);
    // print_str("DESIGN");

    // y += 2;

    // print_set_pos(x, y++);
    // print_str("IJ0 ");
    // print_hex_nibble(i0);
    // print_char(' ');
    // print_hex_nibble(j0);

    // print_set_pos(x, y++);
    // print_str("IJ1 ");
    // print_hex_nibble(i1);
    // print_char(' ');
    // print_hex_nibble(j1);

    // print_set_pos(x, y++);
    // print_str("IJ2 ");
    // print_hex_nibble(i2);
    // print_char(' ');
    // print_hex_nibble(j2);

    // print_set_pos(x, y++);
    // print_str("ACL ");
    // print_hex_nibble(activeL);

    // print_set_pos(x, y++);
    // print_str("ACR ");
    // print_hex_nibble(activeR);

    // print_set_pos(x, y++);
    // print_str("NT  ");
    // print_hex_nibble(nt);
}