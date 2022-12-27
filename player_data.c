#include "player_data.h"
#include "level.h"
#include "board.h"
#include <string.h>
#include <arch/zxn/esxdos.h>
#include <errno.h>

// #define TEST_SOME_UNLOCKED_LEVELS

__asm
_pd_levels_solved:
    defs 90, 0
_pd_levels_padding:
    defs 8, 0
// _pd_levels_unlocked:
//     defb 0
__endasm

extern ubyte pd_levels_solved[];

ubyte pd_all_levels_unlocked = 0;

ubyte pd_base_levels_solved = 0;

ubyte pd_bonus_levels_solved = 0;

uword pd_score = 0;

ubyte pd_debug_on = 0;



#define PD_LEVSOL_BLOCKS 2
#define PD_LEVSOL_MAX PD_LEVSOL_BLOCKS * BOARD_SZ

#define PD_BLOCK_COUNT 80

// extern ubyte pd_levels_solved_persist[];
// extern ubyte pd_level_boards_persist[];

ubyte* pd_levels_solved_persist = 0x0000;
ubyte* pd_level_boards_persist = 0x0062;    // 49 * 2

ubyte* slot_5_address = 0xA000;

const char* pd_filename = "trainyard-express.user";

ubyte* pd_board[BOARD_SZ];

void rom_pagein()
{
    __asm__("NEXTREG $50, 255");
    __asm__("NEXTREG $51, 255");
}

void pd_pagein()
{
    __asm__("NEXTREG $50, 34");
    __asm__("NEXTREG $51, 35");
}

ubyte pd_stored_slot0_page0;
ubyte pd_stored_slot0_page1;

void pd_store_slot0_pages()
{
__asm
    LD A, $50
    CALL ReadNextReg
    LD (_pd_stored_slot0_page0), A

    LD A, $51
    CALL ReadNextReg
    LD (_pd_stored_slot0_page1), A
__endasm;
}

void pd_recall_slot0_pages()
{
__asm
    LD A, (_pd_stored_slot0_page0);
    NEXTREG $50, A

    LD A, (_pd_stored_slot0_page1);
    NEXTREG $51, A
__endasm
}

// set numbered 8K slot to all zeros
void memory_clear_slot(ubyte slotnum)
{
__asm
    LD  HL, 2
    ADD HL, SP
    LD  A, (HL)     ; slotnum
    RRCA
    RRCA
    RRCA            ; 1 -> 0x20, 2->0x40 ...
    LD  H, A
    XOR A
    LD  B, 32
memory_clear_slot_outer:
    LD  C, B
    LD  B, 0        ; == 256 for DNJZ
    LD  L, 0
memory_clear_slot_inner:
    LD (HL), A
    INC L
    DJNZ memory_clear_slot_inner
    INC H
    LD  B, C
    DJNZ memory_clear_slot_outer
__endasm
}

void pd_init_pd_file();

void pd_init()
{
    // put player data page in slots 0, 1
    pd_pagein();

    // clear slots 0, 1
    memory_clear_slot(0);
    memory_clear_slot(1);

    // put rom in slots 0, 1
    rom_pagein();
    // and player data in slot 5
    __asm__("NEXTREG $55, 34");
    // load player data if any, or save empty data
    pd_init_pd_file();
    // recover original bank in slot 5
    __asm__("NEXTREG $55, 5");
    // get player data in slot 0
    pd_pagein();

    // pd_set_sens(pd_sens);

    // copy player data from persist area to work area
    memcpy(pd_levels_solved, pd_levels_solved_persist, PD_LEVSOL_MAX);

    pd_update_score();
}

void pd_update_score()
{
#ifdef TEST_SOME_UNLOCKED_LEVELS
    pd_score = STARS_IN_BASE_LEVELS;
#else
    pd_score = 0;
    for (ubyte i = 0; i < PD_LEVSOL_MAX; i++) {
        ubyte levsol = pd_levels_solved[i];
        if (!levsol) continue;

        ubyte* levrec = levarr[i];
        ubyte levstars = *levrec;
        pd_score += levstars;
    }
#endif
    pd_base_levels_solved = 0;
    if (pd_score >= STARS_IN_BASE_LEVELS)
        pd_base_levels_solved = 1;
    if (pd_score >= STARS_IN_BONUS_LEVELS)
        pd_bonus_levels_solved = 1;
}

void pd_init_pd_file()
{
    ubyte fhnd;
    
    // try to open file for reading
    errno = 0;
    fhnd = esxdos_f_open(pd_filename, ESXDOS_MODE_R | ESXDOS_MODE_OE);
    if (!errno) {
        // file open, try to read
        esxdos_f_read(fhnd, slot_5_address, PD_BLOCK_COUNT * BOARD_SZ);
        if (errno) {
            // cannot read, set area to all zeros
            memset(0xA000, 0, PD_BLOCK_COUNT * BOARD_SZ);
        }
        // close file and return
        esxdos_f_close(fhnd);
        return;
    }

    // file not present, try to create it with all zeros and save
    errno = 0;
    fhnd = esxdos_f_open(pd_filename, ESXDOS_MODE_W | ESXDOS_MODE_CT);
    if (!errno) {
        // file open, try to write
        esxdos_f_write(fhnd, slot_5_address, PD_BLOCK_COUNT * BOARD_SZ);
        if (errno) {
            // whatever
        }
        // close file and return
        esxdos_f_close(fhnd);
        return;
    }
}

void pd_save_levels_solved()
{
    pd_store_slot0_pages();

    rom_pagein();

    errno = 0;
    ubyte fhnd = esxdos_f_open(pd_filename, ESXDOS_MODE_W | ESXDOS_MODE_OE);
    if (!errno) {
        // save levels solved
        esxdos_f_seek(fhnd, 0, ESXDOS_SEEK_SET);
        esxdos_f_write(fhnd, pd_levels_solved, PD_LEVSOL_MAX);

        esxdos_f_close(fhnd);
    }

    pd_recall_slot0_pages();
}

void pd_save_level_board(ubyte levidx, ubyte* srcboard)
{
    pd_store_slot0_pages();

    pd_pagein();

    ubyte* dst = pd_level_boards_persist + levidx * BOARD_SZ;
    memcpy(dst, srcboard, BOARD_SZ);

    rom_pagein();

    errno = 0;
    ubyte fhnd = esxdos_f_open(pd_filename, ESXDOS_MODE_W | ESXDOS_MODE_OE);
    if (!errno) {
        // save levels solved
        esxdos_f_seek(fhnd, 0, ESXDOS_SEEK_SET);
        esxdos_f_write(fhnd, pd_levels_solved, PD_LEVSOL_MAX);
        // save board
        esxdos_f_seek(fhnd, (PD_LEVSOL_BLOCKS + levidx) * BOARD_SZ, ESXDOS_SEEK_SET);
        esxdos_f_write(fhnd, srcboard, BOARD_SZ);

        esxdos_f_close(fhnd);
    }

    pd_recall_slot0_pages();
}

void pd_load_level_board(ubyte levidx, ubyte* dstboard)
{
    pd_store_slot0_pages();

    pd_pagein();

    ubyte* src = pd_level_boards_persist + BOARD_SZ * levidx;
    memcpy(dstboard, src, BOARD_SZ);

    pd_recall_slot0_pages();
}

///////////////////////////////////////////////////////////////////////////////

ubyte* pd_undo_start = 0x2000;

// do not change, it is enforced by bitmask
#define MAX_UNDO_COUNT 0x80
#define UNDO_MASK      0x7F
// #define MAX_UNDO_COUNT 4
// #define UNDO_MASK      3

ubyte pd_snapshot_index = 0;
ubyte pd_snapshot_count = 0;

void pd_clear_undo()
{
    pd_snapshot_index = 0;
    pd_snapshot_count = 0;
}

void pd_snapshot()
{
    pd_store_slot0_pages();

    pd_pagein();

    // snapshot may be written on this slot
    ubyte* snapshot = pd_undo_start + pd_snapshot_index * BOARD_SZ;

    // check if board state is the same as previous snapshot
    // and don't perform a new snapshot in that case
    if (pd_snapshot_count > 0) {
        // address of previous snapshot
        ubyte* prev_snapshot = snapshot - BOARD_SZ;
        if (board_is_equal(board, prev_snapshot))
            return;
    }

    // preform actual snapshot
    board_copy(board, snapshot);

    // increment and wraparound snapshot index
    pd_snapshot_index++;
    pd_snapshot_index &= UNDO_MASK;

    // increment and clamp snapshot count
    pd_snapshot_count++;
    if (pd_snapshot_count > MAX_UNDO_COUNT)
        pd_snapshot_count = MAX_UNDO_COUNT;

    pd_recall_slot0_pages();

}

void pd_undo()
{
    // don't undo when no snapshots available
    if (pd_snapshot_count == 0)
        return;

    pd_store_slot0_pages();

    pd_pagein();
    
    // decrement snapshot count
    pd_snapshot_count--;

    // decrement and wraparound snapshot index
    pd_snapshot_index--;
    pd_snapshot_index &= UNDO_MASK;

    ubyte* snapshot = pd_undo_start + pd_snapshot_index * BOARD_SZ;
    board_copy(snapshot, board);
    board_draw();

    pd_recall_slot0_pages();
}

extern ubyte keyjoy_delta;

ubyte pd_sens = 1;

ubyte pd_debug_reg0A;
void pd_set_sens(ubyte sens)
{
    keyjoy_delta = 1 << sens;

     pd_sens = 2 - sens;
 __asm
    LD A, $0A
    CALL ReadNextReg
    LD (_pd_debug_reg0A), A
    AND A, %11111100
    LD  B, A
    LD  A, (_pd_sens)
    AND A, %00000011
    OR  A, B
    NEXTREG $0A, A;
 __endasm;

    pd_sens = sens;

    // print_set_pos(20, 20);
    // print_hex_byte(pd_debug_reg0A);
}