////////////////////////////////////////////////////////////////////////////////
//
// Tetris for ZX Spectrum 48K
//
// (C) 2019, 2020 David Crespo - https://github.com/dcrespo3d/zx-spectrum-tetris
//
////////////////////////////////////////////////////////////////////////////////
#include "screen.h"
#include "params0.h"

uword screen_get_bmap_addr(ubyte col, ubyte row)
{
	param0b = row;
	param1b = col;

	__asm

		PUSH DE
		PUSH HL

		LD   A, (_param0b)
		LD   D, A
		LD   A, (_param1b)
		LD   E, A
		///////////////////////////////////////
		// D TSRQPONM -> H 010QP000
		// E 76543210 -> L ONM43210
		///////////////////////////////////////
	                      // E = 76543210
		LD   A, E         // A = 76543210
		AND  A, %00011111 // A = 00043210
		LD   L, A         // L = 00043210
		                  // D = TSRQPONM
		LD   A, D         // A = TSRQPONM
		AND  A, %00000111 // A = 00000ONM
		RRCA              // A = M00000ON
		RRCA              // A = NM00000O
		RRCA              // A = ONM00000
		OR   A, L         // A = ONM43210
		LD   L, A         // L = ONM43210 OK
		                  // D = TSRQPONM
		LD   A, D         // A = TSRQPONM
		AND  A, %00011000 // A = 000QP000
		OR   A, %01000000 // A = 010QP000
		LD   H, A         // H = 010QP000 OK

		LD  (_param0w), HL

		POP  HL
		POP  DE

	__endasm

	return param0w;
}

uword screen_get_attr_addr(ubyte col, ubyte row)
{
	param0b = row;
	param1b = col;

	__asm

		PUSH DE
		PUSH HL

		LD   A, (_param0b)
		LD   D, A
		LD   A, (_param1b)
		LD   E, A
		///////////////////////////////////////
		// D TSRQPONM -> H 010110QP
		// E 76543210 -> L ONM43210
		///////////////////////////////////////
	                      // E = 76543210
		LD   A, E         // A = 76543210
		AND  A, %00011111 // A = 00043210
		LD   L, A         // L = 00043210
		                  // D = TSRQPONM
		LD   A, D         // A = TSRQPONM
		AND  A, %00000111 // A = 00000ONM
		RRCA              // A = M00000ON
		RRCA              // A = NM00000O
		RRCA              // A = ONM00000
		OR   A, L         // A = ONM43210
		LD   L, A         // L = ONM43210 OK
		                  // D = TSRQPONM
		LD   A, D         // A = TSRQPONM
		AND  A, %00011000 // A = 000QP000
		RRCA              // A = 0000QP00
		RRCA              // A = 00000QP0
		RRCA              // A = 000000QP
		OR   A, %01011000 // A = 010110QP
		LD   H, A         // H = 010110QP OK

		LD  (_param0w), HL

		POP  HL
		POP  DE

	__endasm

	return param0w;
}

void screen_print_char(ubyte col, ubyte row, ubyte ch)
{
	screen_print_char_raw(screen_get_bmap_addr(col, row), ch);
}

void screen_print_attr(ubyte col, ubyte row, ubyte attr)
{
	ubyte* attr_addr = screen_get_attr_addr(col, row);
	*attr_addr = attr;
}

void screen_print_string(ubyte col, ubyte row, const char* str)
{
	while(0 != *str)
	{
		screen_print_char(col++, row, *str++);
	}
}

void screen_print_char_raw(uword addr, ubyte ch)
{
	param0w = addr;
	param0b = ch;
	__asm
				PUSH HL
				PUSH DE
				PUSH BC

				LD   DE, (_param0w)
				LD   HL, $3C00
				LD   A, (_param0b)
				LD   B, 0
				LD   C, A
				AND  $FF			  // clear the carry bit
				RL   C				  // multiply BC by 8 (shift left 3 times)
				RL   B
				RL   C
				RL   B
				RL   C
				RL   B
	            ADD  HL, BC           // Get the character address in HL
	            LD   C, 8             // Loop counter
screen_print_char_raw1:
                LD   A, (HL)          // Get the byte from the ROM into A
	            LD  (DE), A           // Stick A onto the screen
	            INC  D                // Goto next line on screen
	            INC  L                // Goto next byte of character
	            DEC  C                // Decrease the loop counter
	            JR   NZ, screen_print_char_raw1  // Loop around whilst it is Not Zero (NZ)
				POP  BC
				POP  DE
				POP  HL
	__endasm
}

void screen_clear_bmap(ubyte value)
{
	param0b = value;

	__asm
		PUSH HL
		PUSH BC

		LD   HL, $4000
		LD   C, 0
screen_clear_bmap_outer:
		LD   B, 192
		LD   A, (_param0b)
screen_clear_bmap_inner:
		LD  (HL), A
		INC  HL
		DJNZ screen_clear_bmap_inner
		INC  C
		LD   A, 32
		SUB  A, C
		JR  NZ, screen_clear_bmap_outer

		POP  BC
		POP  HL
	__endasm
}

void screen_clear_attr(ubyte value)
{
	param0b = value;

	__asm
		PUSH HL
		PUSH BC

		LD   HL, $5800
		LD   C, 0
screen_clear_attr_outer:
		LD   B, 32
		LD   A, (_param0b)
screen_clear_attr_inner:
		LD  (HL), A
		INC  HL
		DJNZ screen_clear_attr_inner
		INC  C
		LD   A, 24
		SUB  A, C
		JR  NZ, screen_clear_attr_outer

		POP  BC
		POP  HL
	__endasm
}

void screen_print_user(ubyte col, ubyte row, uword user)
{
	screen_print_user_raw(screen_get_bmap_addr(col, row), user);
}



void screen_print_user_raw(uword addr, uword user)
{
	param0w = addr;
	param1w = user;
	__asm
				PUSH HL
				PUSH DE
				PUSH BC

				LD   DE, (_param0w)
				LD   HL, (_param1w)
	            LD   C, 8             // Loop counter
screen_print_user_raw1:
                LD   A, (HL)          // Get the byte from the ROM into A
	            LD  (DE), A           // Stick A onto the screen
	            INC  D                // Goto next line on screen
	            INC  L                // Goto next byte of character
	            DEC  C                // Decrease the loop counter
	            JR   NZ, screen_print_user_raw1  // Loop around whilst it is Not Zero (NZ)
				POP  BC
				POP  DE
				POP  HL
	__endasm
}

void screen_border_set(ubyte color)
{
	param0b = color;
	__asm
		LD   A, (_param0b)
		OUT ($FE), A
	__endasm
}

__asm
	HL_BLOCK_BMAP_ADDR_FROM_YX_DE:
		///////////////////////////////////////
		// D TSRQPONM -> H 010QP000
		// E 76543210 -> L ONM43210
		///////////////////////////////////////
	                      // E = 76543210
		LD   A, E         // A = 76543210
		AND  A, %00011111 // A = 00043210
		LD   L, A         // L = 00043210
		                  // D = TSRQPONM
		LD   A, D         // A = TSRQPONM
		AND  A, %00000111 // A = 00000ONM
		RRCA              // A = M00000ON
		RRCA              // A = NM00000O
		RRCA              // A = ONM00000
		OR   A, L         // A = ONM43210
		LD   L, A         // L = ONM43210 OK
		                  // D = TSRQPONM
		LD   A, D         // A = TSRQPONM
		AND  A, %00011000 // A = 000QP000
		OR   A, %01000000 // A = 010QP000
		LD   H, A         // H = 010QP000 OK

		RET
__endasm

void screen_clear_bmap_blocks(ubyte value, ubyte x, ubyte y, ubyte w, ubyte h)
{
	param0b = value;
	param1b = x;
	param2b = y;
	param3b = w;
	param4b = h;
	__asm
		PUSH BC
		PUSH DE
		PUSH HL

	// 	LD   A, (_param4b)
	// 	LD   B, A

		LD   B, 0
	screen_clear_bmap_blocks_row:
	 	PUSH BC

		LD   A, (_param2b)
		ADD  A, B
		LD   D, A
		LD   A, (_param1b)
		LD   E, A

		CALL HL_BLOCK_BMAP_ADDR_FROM_YX_DE
	
		LD   A, (_param3b)
		LD   C, A
	
		LD   A, (_param0b)
	screen_clear_bmap_blocks_col:
		LD   B, 8
		LD   D, H
	screen_clear_bmap_blocks_inner:
		LD  (HL), A
		INC  H
		DJNZ screen_clear_bmap_blocks_inner
		LD   H, D

		INC  L
		DEC  C
		JR   NZ, screen_clear_bmap_blocks_col

		POP  BC
		INC  B
		LD   A, (_param4b)
		SUB  A, B
		JR   NZ, screen_clear_bmap_blocks_row

		POP  HL
		POP  DE
		POP  BC
	__endasm
	// for (ubyte j = y; j < y+h; j++)
	// {
	// 	ubyte* addr = (ubyte*)screen_get_bmap_addr(x, j);
	// 	for (ubyte i = 0; i < w; i++)
	// 	{
	// 		ubyte* addr2 = addr;
	// 		for (ubyte k = 0; k < 8; k++)
	// 		{
	// 			*addr2 = value;
	// 			addr2 += 256;
	// 		}
	// 		addr++;
	// 	}
	// }
}

__asm
	HL_BLOCK_ATTR_ADDR_FROM_YX_DE:
		///////////////////////////////////////
		// D TSRQPONM -> H 010110QP
		// E 76543210 -> L ONM43210
		///////////////////////////////////////
	                      // E = 76543210
		LD   A, E         // A = 76543210
		AND  A, %00011111 // A = 00043210
		LD   L, A         // L = 00043210
		                  // D = TSRQPONM
		LD   A, D         // A = TSRQPONM
		AND  A, %00000111 // A = 00000ONM
		RRCA              // A = M00000ON
		RRCA              // A = NM00000O
		RRCA              // A = ONM00000
		OR   A, L         // A = ONM43210
		LD   L, A         // L = ONM43210 OK
		                  // D = TSRQPONM
		LD   A, D         // A = TSRQPONM
		AND  A, %00011000 // A = 000QP000
		RRCA              // A = 0000QP00
		RRCA              // A = 00000QP0
		RRCA              // A = 000000QP
		OR   A, %01011000 // A = 010110QP
		LD   H, A         // H = 010110QP OK

		RET
__endasm

void screen_clear_attr_blocks(ubyte value, ubyte x, ubyte y, ubyte w, ubyte h)
{
	param0b = value;
	param1b = x;
	param2b = y;
	param3b = w;
	param4b = h;
	__asm
		PUSH BC
		PUSH DE
		PUSH HL

		LD   A, (_param2b)
		LD   D, A
		LD   A, (_param1b)
		LD   E, A

		CALL HL_BLOCK_ATTR_ADDR_FROM_YX_DE
		LD   DE, 32

		LD   A, (_param4b)
		LD   C, A
	screen_clear_attr_blocks_outer:
		PUSH HL

		LD   A, (_param3b)
		LD   B, A
	screen_clear_attr_blocks_inner:
		LD   A, (_param0b)
		LD   (HL), A

		INC  L
		DJNZ screen_clear_attr_blocks_inner

		POP  HL
		ADD  HL, DE
		DEC  C
		JR   NZ, screen_clear_attr_blocks_outer

		POP  HL
		POP  DE
		POP  BC
	__endasm

	// for (ubyte j = y; j < y+h; j++)
	// {
	// 	ubyte* addr = (ubyte*)0x5800 + (32 * j) + x;
	// 	for (ubyte i = 0; i < w; i++)
	// 		*addr++ = value;
	// }
}

