////////////////////////////////////////////////////////////////////////////////
//
// Tetris for ZX Spectrum 48K
//
// (C) 2019, 2020 David Crespo - https://github.com/dcrespo3d/zx-spectrum-tetris
//
////////////////////////////////////////////////////////////////////////////////
#include "keyb.h"
#include "print_tile.h"

__asm
._zxkey54321 defb 0
._zxkey67890 defb 0
._zxkeyTREWQ defb 0
._zxkeyYUIOP defb 0
._zxkeyGFDSA defb 0
._zxkeyHJKLe defb 0
._zxkeyVCXZc defb 0
._zxkeyBNMys defb 0
._nxkey0     defb 0
._nxkey1     defb 0
._keyUp      defb 0
._keyDown    defb 0
._keyLeft    defb 0
._keyRight   defb 0
._keyBut1    defb 0
._keyBut2    defb 0

;;----------------------------------------------------------------------------------------
;; reads nextreg in A into A

ReadNextReg:
        ; Input
        ;       A = nextreg to read
        ; Output:
        ;       A = value in nextreg
        ; Uses:
        ;       A, [currently selected NextReg on I/O port $243B]

                push    bc
                ld      bc, $243B   ; TBBLUE_REGISTER_SELECT_P_243B
                out     (c),a
                inc     b       ; bc = TBBLUE_REGISTER_ACCESS_P_253B
                in      a,(c)   ; read desired NextReg state
                pop     bc
                ret
__endasm

void keyb_init()
{
    // Cancel entries in 8x5 matrix for extended keys
    __asm__("NEXTREG $68,$10");
}

void keyb_read_spectrum_next()
{
__asm
    LD A, $B0
    CALL ReadNextReg
    LD (_nxkey0), A

    LD A, $B1
    CALL ReadNextReg
    LD (_nxkey1), A
__endasm
}

void keyb_read_spectrum_std()
{
__asm
    PUSH BC

    LD A, $B0
    CALL ReadNextReg
    LD (_nxkey0), A

    LD A, $B1
    CALL ReadNextReg
    LD (_nxkey1), A

    LD   C, $FE
    LD   B, $F7 ;12345
    IN   A, (C)
    NEG
    DEC  A
    AND  A, $1F
    LD  (_zxkey54321), A

    LD   C, $FE
    LD   B, $EF ;09876
    IN   A, (C)
    NEG
    DEC  A
    AND  A, $1F
    LD  (_zxkey67890), A

    LD   C, $FE
    LD   B, $FB ;QWERT
    IN   A, (C)
    NEG
    DEC  A
    AND  A, $1F
    LD  (_zxkeyTREWQ), A

    LD   C, $FE
    LD   B, $DF ;POIUY
    IN   A, (C)
    NEG
    DEC  A
    AND  A, $1F
    LD  (_zxkeyYUIOP), A

    LD   C, $FE
    LD   B, $FD ;ASDFG
    IN   A, (C)
    NEG
    DEC  A
    AND  A, $1F
    LD  (_zxkeyGFDSA), A

    LD   C, $FE
    LD   B, $BF ;-LKJH
    IN   A, (C)
    NEG
    DEC  A
    AND  A, $1F
    LD  (_zxkeyHJKLe), A

    LD   C, $FE
    LD   B, $FE ;-ZXCV
    IN   A, (C)
    NEG
    DEC  A
    AND  A, $1F
    LD  (_zxkeyVCXZc), A

    LD   C, $FE
    LD   B, $7F ;--MNB
    IN   A, (C)
    NEG
    DEC  A
    AND  A, $1F
    LD  (_zxkeyBNMys), A

    POP BC
__endasm
}

void keyb_print()
{
    print_set_pos(0, 31);
    print_char('1');
    print_hex_byte(zxkey54321);
    print_char(' ');
    print_char('0');
    print_hex_byte(zxkey67890);
    print_char(' ');
    print_char('Q');
    print_hex_byte(zxkeyTREWQ);
    print_char(' ');
    print_char('P');
    print_hex_byte(zxkeyYUIOP);
    print_char(' ');
    print_char('A');
    print_hex_byte(zxkeyGFDSA);
    print_char(' ');
    print_char('e');
    print_hex_byte(zxkeyHJKLe);
    print_char(' ');
    print_char('c');
    print_hex_byte(zxkeyVCXZc);
    print_char(' ');
    print_char('s');
    print_hex_byte(zxkeyBNMys);
    print_char(' ');

    print_char('n');
    print_hex_byte(nxkey0);
    print_char(' ');
    print_char('N');
    print_hex_byte(nxkey1);
    print_char(' ');
}

// ubyte zxkey_any()
// {
//     if (zxkey54321) return 1;
//     if (zxkey67890) return 1;
//     if (zxkeyTREWQ) return 1;
//     if (zxkeyYUIOP) return 1;
//     if (zxkeyGFDSA) return 1;
//     if (zxkeyHJKLe) return 1;
//     if (zxkeyVCXZc) return 1;
//     if (zxkeyBNMys) return 1;
//     return 0;
// }

#define DEF_KEY_DOWN_FUNC(NAME, VAR, MASK) \
ubyte NAME() { \
    __asm__("LD  A, ("#VAR")"); \
    __asm__("AND "#MASK""); \
    __asm__("LD  HL, 0"); \
    __asm__("JR  Z, ASMPC+3"); \
    __asm__("INC HL"); \
}

DEF_KEY_DOWN_FUNC(zxkey1, _zxkey54321, 0x01);
DEF_KEY_DOWN_FUNC(zxkey2, _zxkey54321, 0x02);
DEF_KEY_DOWN_FUNC(zxkey3, _zxkey54321, 0x04);
DEF_KEY_DOWN_FUNC(zxkey4, _zxkey54321, 0x08);
DEF_KEY_DOWN_FUNC(zxkey5, _zxkey54321, 0x10);
DEF_KEY_DOWN_FUNC(zxkey6, _zxkey67890, 0x10);
DEF_KEY_DOWN_FUNC(zxkey7, _zxkey67890, 0x08);
DEF_KEY_DOWN_FUNC(zxkey8, _zxkey67890, 0x04);
DEF_KEY_DOWN_FUNC(zxkey9, _zxkey67890, 0x02);
DEF_KEY_DOWN_FUNC(zxkey0, _zxkey67890, 0x01);

DEF_KEY_DOWN_FUNC(zxkeyQ, _zxkeyTREWQ, 0x01);
DEF_KEY_DOWN_FUNC(zxkeyW, _zxkeyTREWQ, 0x02);
DEF_KEY_DOWN_FUNC(zxkeyE, _zxkeyTREWQ, 0x04);
DEF_KEY_DOWN_FUNC(zxkeyR, _zxkeyTREWQ, 0x08);
DEF_KEY_DOWN_FUNC(zxkeyT, _zxkeyTREWQ, 0x10);
DEF_KEY_DOWN_FUNC(zxkeyY, _zxkeyYUIOP, 0x10);
DEF_KEY_DOWN_FUNC(zxkeyU, _zxkeyYUIOP, 0x08);
DEF_KEY_DOWN_FUNC(zxkeyI, _zxkeyYUIOP, 0x04);
DEF_KEY_DOWN_FUNC(zxkeyO, _zxkeyYUIOP, 0x02);
DEF_KEY_DOWN_FUNC(zxkeyP, _zxkeyYUIOP, 0x01);

DEF_KEY_DOWN_FUNC(zxkeyA, _zxkeyGFDSA, 0x01);
DEF_KEY_DOWN_FUNC(zxkeyS, _zxkeyGFDSA, 0x02);
DEF_KEY_DOWN_FUNC(zxkeyD, _zxkeyGFDSA, 0x04);
DEF_KEY_DOWN_FUNC(zxkeyF, _zxkeyGFDSA, 0x08);
DEF_KEY_DOWN_FUNC(zxkeyG, _zxkeyGFDSA, 0x10);
DEF_KEY_DOWN_FUNC(zxkeyH, _zxkeyHJKLe, 0x10);
DEF_KEY_DOWN_FUNC(zxkeyJ, _zxkeyHJKLe, 0x08);
DEF_KEY_DOWN_FUNC(zxkeyK, _zxkeyHJKLe, 0x04);
DEF_KEY_DOWN_FUNC(zxkeyL, _zxkeyHJKLe, 0x02);
DEF_KEY_DOWN_FUNC(zxkeyENT, _zxkeyHJKLe, 0x01);

DEF_KEY_DOWN_FUNC(zxkeyCAP, _zxkeyVCXZc, 0x01);
DEF_KEY_DOWN_FUNC(zxkeyZ, _zxkeyVCXZc, 0x02);
DEF_KEY_DOWN_FUNC(zxkeyX, _zxkeyVCXZc, 0x04);
DEF_KEY_DOWN_FUNC(zxkeyC, _zxkeyVCXZc, 0x08);
DEF_KEY_DOWN_FUNC(zxkeyV, _zxkeyVCXZc, 0x10);
DEF_KEY_DOWN_FUNC(zxkeyB, _zxkeyBNMys, 0x10);
DEF_KEY_DOWN_FUNC(zxkeyN, _zxkeyBNMys, 0x08);
DEF_KEY_DOWN_FUNC(zxkeyM, _zxkeyBNMys, 0x04);
DEF_KEY_DOWN_FUNC(zxkeySYM, _zxkeyBNMys, 0x02);
DEF_KEY_DOWN_FUNC(zxkeySPC, _zxkeyBNMys, 0x01);

DEF_KEY_DOWN_FUNC(nxkeyUp, _nxkey0, 0x08);
DEF_KEY_DOWN_FUNC(nxkeyDown, _nxkey0, 0x04);
DEF_KEY_DOWN_FUNC(nxkeyLeft, _nxkey0, 0x02);
DEF_KEY_DOWN_FUNC(nxkeyRight, _nxkey0, 0x01);
DEF_KEY_DOWN_FUNC(nxkeyDel, _nxkey1, 0x80);

void keyb_update()
{
    keyb_read_spectrum_std();
    keyb_read_spectrum_next();

    keyUp  = zxkeyQ();
    keyUp |= nxkeyUp();

    keyDown  = zxkeyA();
    keyDown |= nxkeyDown();

    keyLeft  = zxkeyO();
    keyLeft |= nxkeyLeft();

    keyRight  = zxkeyP();
    keyRight |= nxkeyRight();

    keyBut1  = zxkeySPC();

    keyBut2  = zxkey0();
    keyBut2 |= nxkeyDel();
}

