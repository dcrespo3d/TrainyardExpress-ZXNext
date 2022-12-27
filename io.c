#include "io.h"
#include "types.h"
#include "board.h"
#include <arch/zxn/esxdos.h>
#include <errno.h>

ubyte io_board[BOARD_SZ];

void io_mmu_page_rom_in()
{
__asm
    NEXTREG $50, 255
    NEXTREG $51, 255
__endasm
}

void io_board_store()
{
    io_mmu_page_rom_in();

    board_copy_tracks(board, io_board);

    errno = 0;
    ubyte filehandle = esxdos_f_open("board_dump.raw",
        ESXDOS_MODE_W | ESXDOS_MODE_CT);
    if (errno)
        return;
    esxdos_f_write(filehandle, io_board, BOARD_SZ);
    esxdos_f_close(filehandle);
}

void io_board_recall()
{
    io_mmu_page_rom_in();
    
    errno = 0;
    ubyte filehandle = esxdos_f_open("board_dump.raw",
        ESXDOS_MODE_R | ESXDOS_MODE_OE);
    if (errno)
        return;
    esxdos_f_read(filehandle, io_board, BOARD_SZ);
    esxdos_f_close(filehandle);

    board_copy_tracks(io_board, board);

    board_draw();
}

void io_test_big_save()
{
    io_mmu_page_rom_in();

    errno = 0;
    ubyte filehandle = esxdos_f_open("big_dump.raw",
        ESXDOS_MODE_W | ESXDOS_MODE_CT);
    if (errno)
        return;
    esxdos_f_write(filehandle, 0xA000, 0x4000);
    esxdos_f_close(filehandle);
}

void io_test_big_load()
{
    errno = 0;
    ubyte filehandle = esxdos_f_open("board_dump.raw",
        ESXDOS_MODE_R | ESXDOS_MODE_OE);
    if (errno)
        return;
    esxdos_f_read(filehandle, 0xA000, 0x4000);
    esxdos_f_close(filehandle);
}
