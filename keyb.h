////////////////////////////////////////////////////////////////////////////////
//
// Tetris for ZX Spectrum 48K
//
// (C) 2019, 2020 David Crespo - https://github.com/dcrespo3d/zx-spectrum-tetris
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __zxkeyb_h__
#define __zxkeyb_h__

#include "types.h"

void keyb_init();
void keyb_update();

void keyb_print();

extern ubyte zxkey54321;
extern ubyte zxkey67890;
extern ubyte zxkeyTREWQ;
extern ubyte zxkeyYUIOP;
extern ubyte zxkeyGFDSA;
extern ubyte zxkeyHJKLe;
extern ubyte zxkeyVCXZc;
extern ubyte zxkeyBNMys;
extern ubyte nxkey0;
extern ubyte nxkey1;

extern ubyte keyLeft;  // left
extern ubyte keyRight; // right
extern ubyte keyUp;    // up
extern ubyte keyDown;  // down
extern ubyte keyBut1;  // button 1 (left)
extern ubyte keyBut2;  // button 2 (right)

// ubyte zxkey_any();

ubyte zxkey1();
ubyte zxkey2();
ubyte zxkey3();
ubyte zxkey4();
ubyte zxkey5();
ubyte zxkey6();
ubyte zxkey7();
ubyte zxkey8();
ubyte zxkey9();
ubyte zxkey0();

ubyte zxkeyQ();
ubyte zxkeyW();
ubyte zxkeyE();
ubyte zxkeyR();
ubyte zxkeyT();
ubyte zxkeyY();
ubyte zxkeyU();
ubyte zxkeyI();
ubyte zxkeyO();
ubyte zxkeyP();

ubyte zxkeyA();
ubyte zxkeyS();
ubyte zxkeyD();
ubyte zxkeyF();
ubyte zxkeyG();
ubyte zxkeyH();
ubyte zxkeyJ();
ubyte zxkeyK();
ubyte zxkeyL();
ubyte zxkeyENT();

ubyte zxkeyCAP();
ubyte zxkeyZ();
ubyte zxkeyX();
ubyte zxkeyC();
ubyte zxkeyV();
ubyte zxkeyB();
ubyte zxkeyN();
ubyte zxkeyM();
ubyte zxkeySYM();
ubyte zxkeySPC();

ubyte nxkeyUp();
ubyte nxkeyDown();
ubyte nxkeyLeft();
ubyte nxkeyRight();
ubyte nxkeyDel();


#endif // _zxkeyb_h_
