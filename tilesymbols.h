#ifndef __tilesymbols_h__
#define __tilesymbols_h__

#define XXXX 0x00
#define XXNW 0x30
#define XXNS 0x50
#define XXWS 0x60
#define XXNE 0x90
#define XXWE 0xA0
#define XXSE 0xC0
#define NWXX 0x03
#define NWNW 0x33
#define NWNS 0x53
#define NWWS 0x63
#define NWNE 0x93
#define NWWE 0xA3
#define NWSE 0xC3
#define NSXX 0x05
#define NSNW 0x35
#define NSNS 0x55
#define NSWS 0x65
#define NSNE 0x95
#define NSWE 0xA5
#define NSSE 0xC5
#define WSXX 0x06
#define WSNW 0x36
#define WSNS 0x56
#define WSWS 0x66
#define WSNE 0x96
#define WSWE 0xA6
#define WSSE 0xC6
#define NEXX 0x09
#define NENW 0x39
#define NENS 0x59
#define NEWS 0x69
#define NENE 0x99
#define NEWE 0xA9
#define NESE 0xC9
#define WEXX 0x0A
#define WENW 0x3A
#define WENS 0x5A
#define WEWS 0x6A
#define WENE 0x9A
#define WEWE 0xAA
#define WESE 0xCA
#define SEXX 0x0C
#define SENW 0x3C
#define SENS 0x5C
#define SEWS 0x6C
#define SENE 0x9C
#define SEWE 0xAC
#define SESE 0xCC

// #define XXXX 0x00
// #define XXNW 0x03
// #define XXNS 0x05
// #define XXWS 0x06
// #define XXNE 0x09
// #define XXWE 0x0A
// #define XXSE 0x0C
// #define NWXX 0x30
// #define NWNW 0x33
// #define NWNS 0x35
// #define NWWS 0x36
// #define NWNE 0x39
// #define NWWE 0x3A
// #define NWSE 0x3C
// #define NSXX 0x50
// #define NSNW 0x53
// #define NSNS 0x55
// #define NSWS 0x56
// #define NSNE 0x59
// #define NSWE 0x5A
// #define NSSE 0x5C
// #define WSXX 0x60
// #define WSNW 0x63
// #define WSNS 0x65
// #define WSWS 0x66
// #define WSNE 0x69
// #define WSWE 0x6A
// #define WSSE 0x6C
// #define NEXX 0x90
// #define NENW 0x93
// #define NENS 0x95
// #define NEWS 0x96
// #define NENE 0x99
// #define NEWE 0x9A
// #define NESE 0x9C
// #define WEXX 0xA0
// #define WENW 0xA3
// #define WENS 0xA5
// #define WEWS 0xA6
// #define WENE 0xA9
// #define WEWE 0xAA
// #define WESE 0xAC
// #define SEXX 0xC0
// #define SENW 0xC3
// #define SENS 0xC5
// #define SEWS 0xC6
// #define SENE 0xC9
// #define SEWE 0xCA
// #define SESE 0xCC

// #define SRC0 0xF0
// #define SRC1 0xF1
// #define SRC2 0xF2
// #define SRC3 0xF3
// #define SRC4 0xF4
// #define SRC5 0xF5
// #define SRC6 0xF6
// #define SRC7 0xF7
// #define SRC8 0xF8
// #define SRC9 0xF9
// #define SRCA 0xFA
// #define SRCB 0xFB
// #define SRCC 0xFC
// #define SRCD 0xFD
// #define SRCE 0xFE
// #define SRCF 0xFF

// #define SNK0 0xE0
// #define SNK1 0xE1
// #define SNK2 0xE2
// #define SNK3 0xE3
// #define SNK4 0xE4
// #define SNK5 0xE5
// #define SNK6 0xE6
// #define SNK7 0xE7
// #define SNK8 0xE8
// #define SNK9 0xE9
// #define SNKA 0xEA
// #define SNKB 0xEB
// #define SNKC 0xEC
// #define SNKD 0xED
// #define SNKE 0xEE
// #define SNKF 0xEF

#define SRCN 0xF1
#define SRCW 0xF2
#define SRCS 0xF4
#define SRCE 0xF8

#define SNKN 0xE1
#define SNKW 0xE2
#define SNKS 0xE4
#define SNKE 0xE8

#define SNNW 0xE3
#define SNNS 0xE5
#define SNWS 0xE6
#define SNNE 0xE9
#define SNWE 0xEA
#define SNSE 0xEC

#define OBS0 0xD0

#define TILE_VOID 25

#define TILE_BLNK 36
#define TILE_SRCN 37
#define TILE_SRCW 38
#define TILE_SRCS 39
#define TILE_SRCE 40
#define TILE_OBST 41
#define TILE_SNNW 42
#define TILE_SNNS 43
#define TILE_SNWS 44
#define TILE_SNNE 45
#define TILE_SNWE 46
#define TILE_SNSE 47
#define TILE_SNAL 48
#define TILE_SNKN 49
#define TILE_SNKW 50
#define TILE_SNKS 51
#define TILE_SNKE 52

#define TILE_ERR 53

#define TILE_KWSE 55
#define TILE_KNSE 56
#define TILE_KNWE 57
#define TILE_KNWS 58


#define DIRN 1
#define DIRW 2
#define DIRS 4
#define DIRE 8
#define DIRA 15

#define XX 0
#define NW 3
#define NS 5
#define WS 6
#define NE 9
#define WE 10
#define SE 12

// scissor
#define SCI 7
// painter
#define PNT 11
// obstacle
#define OBS 13
// sink
#define SNK 14
// source
#define SRC 15

/*

0   0  0000   0   EMPTY
1   1  0001   1   N
2   2  0010   1   W
3   3  0011   2   NW
4   4  0100   1   S
5   5  0101   2   NS
6   6  0110   2   WS
7   7  0111   3   SCI
8   8  1000   1   E
9   9  1001   2   NE
A  10  1010   2   WE
B  11  1011   3   PNT
C  12  1100   2   SE
D  13  1101   3   OBS
E  14  1110   3   SNK
F  15  1111   4   SRC

*/

#endif // __tilesymbols_h__
