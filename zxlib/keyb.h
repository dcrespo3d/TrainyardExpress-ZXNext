////////////////////////////////////////////////////////////////////////////////
//
// Tetris for ZX Spectrum 48K
//
// (C) 2019, 2020 David Crespo - https://github.com/dcrespo3d/zx-spectrum-tetris
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _keyb_h_
#define _keyb_h_

#include "types.h"

void keyb_read();

void keybToScreen();

extern ubyte zxkey54321;
extern ubyte zxkey67890;
extern ubyte zxkeyTREWQ;
extern ubyte zxkeyYUIOP;
extern ubyte zxkeyGFDSA;
extern ubyte zxkeyHJKLe;
extern ubyte zxkeyVCXZc;
extern ubyte zxkeyBNMys;

ubyte keyb_any_down();

ubyte key1down();
ubyte key2down();
ubyte key3down();
ubyte key4down();
ubyte key5down();
ubyte key6down();
ubyte key7down();
ubyte key8down();
ubyte key9down();
ubyte key0down();

ubyte keyQdown();
ubyte keyWdown();
ubyte keyEdown();
ubyte keyRdown();
ubyte keyTdown();
ubyte keyYdown();
ubyte keyUdown();
ubyte keyIdown();
ubyte keyOdown();
ubyte keyPdown();

ubyte keyAdown();
ubyte keySdown();
ubyte keyDdown();
ubyte keyFdown();
ubyte keyGdown();
ubyte keyHdown();
ubyte keyJdown();
ubyte keyKdown();
ubyte keyLdown();
ubyte keyENTdown();

ubyte keyCAPdown();
ubyte keyZdown();
ubyte keyXdown();
ubyte keyCdown();
ubyte keyVdown();
ubyte keyBdown();
ubyte keyNdown();
ubyte keyMdown();
ubyte keySYMdown();
ubyte keySPCdown();

#endif // _keyb_h_
