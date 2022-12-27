////////////////////////////////////////////////////////////////////////////////
//
// Tetris for ZX Spectrum 48K
//
// (C) 2019, 2020 David Crespo - https://github.com/dcrespo3d/zx-spectrum-tetris
//
////////////////////////////////////////////////////////////////////////////////
#include "keyb.h"

#asm
._zxkey54321
defb 0
._zxkey67890
defb 0
._zxkeyTREWQ
defb 0
._zxkeyYUIOP
defb 0
._zxkeyGFDSA
defb 0
._zxkeyHJKLe
defb 0
._zxkeyVCXZc
defb 0
._zxkeyBNMys
defb 0
#endasm

void keyb_read()
{
    __asm
        PUSH BC

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

void keybToScreen()
{
    __asm
        PUSH BC

        LD   C, $FE
        LD   B, $F7 ;12345
        IN   A, (C)
        NEG
        DEC  A
        AND  A, $1F
        LD  ($4000), A

        LD   C, $FE
        LD   B, $EF ;09876
        IN   A, (C)
        NEG
        DEC  A
        AND  A, $1F
        LD  ($4001), A

        LD   C, $FE
        LD   B, $FB ;QWERT
        IN   A, (C)
        NEG
        DEC  A
        AND  A, $1F
        LD  ($4200), A

        LD   C, $FE
        LD   B, $DF ;POIUY
        IN   A, (C)
        NEG
        DEC  A
        AND  A, $1F
        LD  ($4201), A

        LD   C, $FE
        LD   B, $FD ;ASDFG
        IN   A, (C)
        NEG
        DEC  A
        AND  A, $1F
        LD  ($4400), A

        LD   C, $FE
        LD   B, $BF ;-LKJH
        IN   A, (C)
        NEG
        DEC  A
        AND  A, $1F
        LD  ($4401), A

        LD   C, $FE
        LD   B, $FE ;-ZXCV
        IN   A, (C)
        NEG
        DEC  A
        AND  A, $1F
        LD  ($4600), A

        LD   C, $FE
        LD   B, $7F ;--MNB
        IN   A, (C)
        NEG
        DEC  A
        AND  A, $1F
        LD  ($4601), A

        POP BC
    __endasm
}

ubyte keyb_any_down()
{
    if (zxkey54321) return 1;
    if (zxkey67890) return 1;
    if (zxkeyTREWQ) return 1;
    if (zxkeyYUIOP) return 1;
    if (zxkeyGFDSA) return 1;
    if (zxkeyHJKLe) return 1;
    if (zxkeyVCXZc) return 1;
    if (zxkeyBNMys) return 1;
    return 0;
}

ubyte key1down() { return zxkey54321 & 0x01 ? 1 : 0; }
ubyte key2down() { return zxkey54321 & 0x02 ? 1 : 0; }
ubyte key3down() { return zxkey54321 & 0x04 ? 1 : 0; }
ubyte key4down() { return zxkey54321 & 0x08 ? 1 : 0; }
ubyte key5down() { return zxkey54321 & 0x10 ? 1 : 0; }
ubyte key6down() { return zxkey67890 & 0x10 ? 1 : 0; }
ubyte key7down() { return zxkey67890 & 0x08 ? 1 : 0; }
ubyte key8down() { return zxkey67890 & 0x04 ? 1 : 0; }
ubyte key9down() { return zxkey67890 & 0x02 ? 1 : 0; }
ubyte key0down() { return zxkey67890 & 0x01 ? 1 : 0; }

ubyte keyQdown() { return zxkeyTREWQ & 0x01 ? 1 : 0; }
ubyte keyWdown() { return zxkeyTREWQ & 0x02 ? 1 : 0; }
ubyte keyEdown() { return zxkeyTREWQ & 0x04 ? 1 : 0; }
ubyte keyRdown() { return zxkeyTREWQ & 0x08 ? 1 : 0; }
ubyte keyTdown() { return zxkeyTREWQ & 0x10 ? 1 : 0; }
ubyte keyYdown() { return zxkeyYUIOP & 0x10 ? 1 : 0; }
ubyte keyUdown() { return zxkeyYUIOP & 0x08 ? 1 : 0; }
ubyte keyIdown() { return zxkeyYUIOP & 0x04 ? 1 : 0; }
ubyte keyOdown() { return zxkeyYUIOP & 0x02 ? 1 : 0; }
ubyte keyPdown() { return zxkeyYUIOP & 0x01 ? 1 : 0; }

ubyte keyAdown()   { return zxkeyGFDSA & 0x01 ? 1 : 0; }
ubyte keySdown()   { return zxkeyGFDSA & 0x02 ? 1 : 0; }
ubyte keyDdown()   { return zxkeyGFDSA & 0x04 ? 1 : 0; }
ubyte keyFdown()   { return zxkeyGFDSA & 0x08 ? 1 : 0; }
ubyte keyGdown()   { return zxkeyGFDSA & 0x10 ? 1 : 0; }
ubyte keyHdown()   { return zxkeyHJKLe & 0x10 ? 1 : 0; }
ubyte keyJdown()   { return zxkeyHJKLe & 0x08 ? 1 : 0; }
ubyte keyKdown()   { return zxkeyHJKLe & 0x04 ? 1 : 0; }
ubyte keyLdown()   { return zxkeyHJKLe & 0x02 ? 1 : 0; }
ubyte keyENTdown() { return zxkeyHJKLe & 0x01 ? 1 : 0; }

ubyte keyCAPdown() { return zxkeyVCXZc & 0x01 ? 1 : 0; }
ubyte keyZdown()   { return zxkeyVCXZc & 0x02 ? 1 : 0; }
ubyte keyXdown()   { return zxkeyVCXZc & 0x04 ? 1 : 0; }
ubyte keyCdown()   { return zxkeyVCXZc & 0x08 ? 1 : 0; }
ubyte keyVdown()   { return zxkeyVCXZc & 0x10 ? 1 : 0; }
ubyte keyBdown()   { return zxkeyBNMys & 0x10 ? 1 : 0; }
ubyte keyNdown()   { return zxkeyBNMys & 0x08 ? 1 : 0; }
ubyte keyMdown()   { return zxkeyBNMys & 0x04 ? 1 : 0; }
ubyte keySYMdown() { return zxkeyBNMys & 0x02 ? 1 : 0; }
ubyte keySPCdown() { return zxkeyBNMys & 0x01 ? 1 : 0; }
