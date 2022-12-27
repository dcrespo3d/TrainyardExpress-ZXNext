////////////////////////////////////////////////////////////////////////////////
//
// Tetris for ZX Spectrum 48K
//
// (C) 2019, 2020 David Crespo - https://github.com/dcrespo3d/zx-spectrum-tetris
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _colors_h_
#define _colors_h_

// basic colors
#define ZX_BLK 0x00
#define ZX_BLU 0x01
#define ZX_RED 0x02
#define ZX_MAG 0x03
#define ZX_GRE 0x04
#define ZX_CYA 0x05
#define ZX_YEL 0x06
#define ZX_WHI 0x07

// bright and flash bits
#define ZX_BRI 0x40
#define ZX_FLA 0x80

// macro for creating 
#define ZX_COLOR(PAPER,INK) (((PAPER&7)<<3)|(INK&7))

// all possible paper/ink color combinations
#define ZX_BLK_BLK 0x00
#define ZX_BLK_BLU 0x01
#define ZX_BLK_RED 0x02
#define ZX_BLK_MAG 0x03
#define ZX_BLK_GRE 0x04
#define ZX_BLK_CYA 0x05
#define ZX_BLK_YEL 0x06
#define ZX_BLK_WHI 0x07
#define ZX_BLU_BLK 0x08
#define ZX_BLU_BLU 0x09
#define ZX_BLU_RED 0x0A
#define ZX_BLU_MAG 0x0B
#define ZX_BLU_GRE 0x0C
#define ZX_BLU_CYA 0x0D
#define ZX_BLU_YEL 0x0E
#define ZX_BLU_WHI 0x0F
#define ZX_RED_BLK 0x10
#define ZX_RED_BLU 0x11
#define ZX_RED_RED 0x12
#define ZX_RED_MAG 0x13
#define ZX_RED_GRE 0x14
#define ZX_RED_CYA 0x15
#define ZX_RED_YEL 0x16
#define ZX_RED_WHI 0x17
#define ZX_MAG_BLK 0x18
#define ZX_MAG_BLU 0x19
#define ZX_MAG_RED 0x1A
#define ZX_MAG_MAG 0x1B
#define ZX_MAG_GRE 0x1C
#define ZX_MAG_CYA 0x1D
#define ZX_MAG_YEL 0x1E
#define ZX_MAG_WHI 0x1F
#define ZX_GRE_BLK 0x20
#define ZX_GRE_BLU 0x21
#define ZX_GRE_RED 0x22
#define ZX_GRE_MAG 0x23
#define ZX_GRE_GRE 0x24
#define ZX_GRE_CYA 0x25
#define ZX_GRE_YEL 0x26
#define ZX_GRE_WHI 0x27
#define ZX_CYA_BLK 0x28
#define ZX_CYA_BLU 0x29
#define ZX_CYA_RED 0x2A
#define ZX_CYA_MAG 0x2B
#define ZX_CYA_GRE 0x2C
#define ZX_CYA_CYA 0x2D
#define ZX_CYA_YEL 0x2E
#define ZX_CYA_WHI 0x2F
#define ZX_YEL_BLK 0x30
#define ZX_YEL_BLU 0x31
#define ZX_YEL_RED 0x32
#define ZX_YEL_MAG 0x33
#define ZX_YEL_GRE 0x34
#define ZX_YEL_CYA 0x35
#define ZX_YEL_YEL 0x36
#define ZX_YEL_WHI 0x37
#define ZX_WHI_BLK 0x38
#define ZX_WHI_BLU 0x39
#define ZX_WHI_RED 0x3A
#define ZX_WHI_MAG 0x3B
#define ZX_WHI_GRE 0x3C
#define ZX_WHI_CYA 0x3D
#define ZX_WHI_YEL 0x3E
#define ZX_WHI_WHI 0x3F

/* python 3 code for generating all previous color combinations
colors=(
    ('BLK',0),
    ('BLU',1),
    ('RED',2),
    ('MAG',3),
    ('GRE',4),
    ('CYA',5),
    ('YEL',6),
    ('WHI',7))

for color1 in colors:
    cname1,cval1 = color1
    for color2 in colors:
        cname2,cval2 = color2
        cval = cval2 | (cval1 << 3)
        cvalhex = "0x%0.2X" % cval
        s = '#define ZX_'+cname1+'_'+cname2+' '+cvalhex
        print(s)
*/

#endif // _colors_h_
