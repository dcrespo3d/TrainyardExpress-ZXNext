#include "sound.h"
#include "sound_freqs.h"
#include "params.h"
#include "print_tile.h"
#include "types.h"

// 2 - RED - C
// 4 - YEL - D
// 1 - BLU - E
// 6 - ORA - F
// 5 - GRE - G
// 3 - MAG - A
//
// BLU - 1 - E
// RED - 2 - C
// MAG - 3 - A
// YEL - 4 - D
// GRE - 5 - G
// ORA - 6 - F

// The AY-3-8910 has 16 registers:
// Register  Purpose                   Width
// -------------------------------------------
// 0         Channel A fine pitch      8 bits
// 1         Channel A coarse pitch    4 bits
// 2         Channel B fine pitch      8 bits
// 3         Channel B coarse pitch    4 bits
// 4         Channel C fine pitch      8 bits
// 5         Channel C coarse pitch    4 bits
// 6         Noise pitch               5 bits
// 7         Mixer                     8 bits
// 8         Channel A volume          4 bits
// 9         Channel B volume          4 bits
// 10        Channel C volume          4 bits
// 11        Envelope fine duration    8 bits
// 12        Envelope coarse duration  8 bits
// 13        Envelope shape            4 bits
// 14        I/O port A                8 bits
// 15        I/O port B                8 bits
#define REG_PITCH_A     0
#define REG_PITCH_B     2
#define REG_PITCH_C     4
#define REG_NOISE_PITCH 6
#define REG_MIXER       7
#define REG_VOL_A       8
#define REG_VOL_B       9
#define REG_VOL_C       10

#define SWITCH_NOISE_PITCH 0
#define SWITCH_VOL 15
#define SWITCH_TONE_PITCH 20

// tone slot count
#define TS_CNT 6
// noise slot count
#define NS_CNT 3

#define AY1 0b01
#define AY2 0b10
#define AY3 0b11

#define TONE_A     0b000001
#define TONE_B     0b000010
#define TONE_C     0b000100

#define TONE_MASK  0b000111

#define NOISE_A    0b001000
#define NOISE_B    0b010000
#define NOISE_C    0b100000

#define NOISE_MASK 0b111000

#define TYP_PIANO 0
#define TYP_BRASS 1

ubyte ts_ids[TS_CNT] = {
    0b01000001,
    0b01000010,
    0b10000001,
    0b10000010,
    0b11000001,
    0b11000010
};

ubyte ns_ids[NS_CNT] = {
    0b01100000,
    0b10100000,
    0b11100000
};

// volume
sbyte ts_vol[TS_CNT] = {
    0, 0, 0, 0, 0, 0
};

// type
ubyte ts_typ[TS_CNT] = {
    0, 0, 0, 0, 0, 0
};

// period
uword ts_per[TS_CNT] = {
    0, 0, 0, 0, 0, 0
};

// frame
ubyte ts_fra[TS_CNT] = {
    0, 0, 0, 0, 0, 0
};

#define CRASH_AYN 1
#define CRASH_NEG_MASK 0b11011111
#define CRASH_POS_MASK 0b00100000

sbyte crash_vol = 0;
ubyte crash_pit = 0;

#define SWITCH_AYN 2
#define SWITCH_NEG_MASK 0b11011011
#define SWITCH_POS_MASK 0b00100100

sbyte switch_vol = 0;
ubyte switch_pit = 0;

__asm
; A: reg
; D: val
; BC: clobbered
; return: A, val
snd_get_ay_reg8:
    LD BC, $FFFD
    OUT (C), A
    IN  A, (C)
    RET
__endasm;

ubyte snd_get_ay_reg8(ubyte reg)
{
__asm
    LD  HL, 2
    ADD HL, SP
    LD  A,  (HL)    ; A <- reg

	; Select desired register
	LD BC, $FFFD
	OUT (C), A

	; Read value
    IN A, (C)

    ; Return value
    LD L, A
    LD H, 0
__endasm;
}

__asm
; A: reg
; D: val
; BC: clobbered
snd_put_ay_reg8:
    LD BC, $FFFD
    OUT (C), A

    LD BC, $BFFD
    OUT (C), D

    RET

__endasm;

void snd_put_ay_reg8(ubyte reg, ubyte val)
{
__asm
    LD  HL, 4
    ADD HL, SP
    LD  A,  (HL)    ; A <- reg

	; Select desired register
	LD BC, $FFFD
	OUT (C), A

    LD  HL, 2
    ADD HL, SP
    LD  A,  (HL)    ; A <- val

	; Write given value
	LD BC, $BFFD
	OUT (C), A
__endasm;
}

__asm
; A: reg
; DE: val
; BC: clobbered
; L: clobbered
snd_put_ay_reg16:
    LD BC, $FFFD
    OUT (C), A

    LD BC, $BFFD
    OUT (C), E

    INC A

    LD BC, $FFFD
    OUT (C), A

    LD BC, $BFFD
    OUT (C), D

    RET

__endasm;

void snd_put_ay_reg16(ubyte reg, uword val)
{
    param6b = reg;
    param7w = val;
__asm
    POP HL
    POP DE      ; DE <- val
    PUSH DE
    PUSH HL

    LD  HL, 4
    ADD HL, SP
    LD  A,  (HL)    ; L <- reg

	; // select register
	LD  BC, $FFFD
	OUT (C), A

	; // write value
	LD  BC, $BFFD
	OUT (C), E

    // next register
    INC A

	; // select register
	LD  BC, $FFFD
	OUT (C), A

	; // write value
	LD  BC, $BFFD
	OUT (C), D

__endasm;
}

__asm
; A: idx
; BC: clobbered
snd_select_ay:
    AND A, %00000011
    OR  A, %11111100
    LD  BC, $FFFD
    OUT (C), A
    RET
__endasm;

void snd_select_ay(ubyte idx)
{
    param5b = idx;
__asm
    LD  HL, 2
    ADD HL, SP
    LD  A, (HL)

    AND A, %00000011
    OR  A, %11111100

	; Setup Turbo Sound chip
	LD BC, $FFFD		; Turbo Sound Next Control Register
	OUT (C), A
__endasm;
}

void snd_init()
{
__asm
	; Setup Turbo Sound chip
	LD BC, $FFFD		; Turbo Sound Next Control Register
	LD A, %11111101		; Enable left+right audio, select AY1
	OUT (C), A

	; Setup mapping of chip channels to stereo channels
	NEXTREG $08, %00010010	; Use ABC, enable internal speaker & turbosound
	NEXTREG $09, %11100000	; Enable mono for AY0-2
__endasm

    snd_panic();
}

void snd_panic()
{
    // snd_select_ay(1);
    // snd_put_ay_reg8(REG_MIXER, 0xFF);
    // snd_select_ay(2);
    // snd_put_ay_reg8(REG_MIXER, 0xFF);
    // snd_select_ay(3);
    // snd_put_ay_reg8(REG_MIXER, 0xFF);
__asm
    LD D, $FF
    LD A, AY1
    CALL snd_select_ay
    LD A, REG_MIXER
    CALL snd_put_ay_reg8

    LD A, AY2
    CALL snd_select_ay
    LD A, REG_MIXER
    CALL snd_put_ay_reg8

    LD A, AY3
    CALL snd_select_ay
    LD A, REG_MIXER
    CALL snd_put_ay_reg8
__endasm
}

void snd_init_tone_slot(ubyte idx, uword period);
void snd_update_tone_slot(ubyte idx);

void snd_init_crash_slot();
void snd_update_crash_slot();

void snd_init_switch_slot();
void snd_update_switch_slot();


void snd_update()
{
    for (ubyte i = 0; i < TS_CNT; i++) {
        if (ts_vol[i] != 0) {
            snd_update_tone_slot(i);
        }
    }
    if (crash_vol)
        snd_update_crash_slot();
    if (switch_vol)
        snd_update_switch_slot();
}

uword color_notes[8] = {
	// T_C3, T_C4, T_E4, T_A4, T_D4, T_F4, T_G4, T_B4
	T_C3, T_E5, T_C5, T_A5, T_D5, T_G5, T_F5, T_B5
};

void snd_select_ay(ubyte idx);

void snd_play_color(ubyte color)
{
	color &= 7;
    uword per = color_notes[color];
    snd_play_tone(per);
}

void snd_play_tone(uword per)
{
    for (ubyte i = 0; i < TS_CNT; i++) {
        if (ts_vol[i] == 0) {
            snd_init_tone_slot(i, per);
            return;
        }
    }
}

void snd_play_crash()
{
    snd_init_crash_slot();
}

void snd_play_switch()
{
    snd_init_switch_slot();
}

void snd_play_success()
{
    snd_play_tone(T_C4);
    snd_play_tone(T_G4);
    snd_play_tone(T_C5);
    snd_play_tone(T_E5);
}


// ubyte mixval = 

void snd_init_tone_slot(ubyte idx, uword per)
{
    // ubyte id = ts_ids[idx];

    // ubyte ayn = id >> 6;
    // snd_select_ay(ayn);

    // ubyte mask = ~(id & 0b11);
    // ubyte mixval = snd_get_ay_reg8(REG_MIXER);
    // mixval &= mask;
    // snd_put_ay_reg8(REG_MIXER, mixval);

    // // pitch reg num
    // ubyte prnum = id & 0b10;
	// snd_put_ay_reg16(prnum, per);

    // ubyte vol = 127;
    // ts_vol[idx] = vol;

    // ubyte chnum = 7 + (id & 0b11);
    // snd_put_ay_reg8(chnum, vol >> 3);

    // print_set_pos(10, 10+idx);
    // print_hex_byte(id);
    // print_char(' ');

    // print_hex_byte(ayn);
    // print_char(' ');

    // print_hex_byte(mixval);
    // print_char(' ');

    // print_hex_byte(prnum);
    // print_char(' ');

    // print_hex_byte(chnum);
    // print_char(' ');

    // print_hex_byte(vol);
    // print_char(' ');

    param0b = idx;
    param1w = per;

__asm

    LD  HL, 4
    ADD HL, SP
    LD  A, (HL)     ; idx
    LD  D,  0
    LD  E,  A

    POP BC
    POP HL          ; per
    PUSH HL
    PUSH BC

    PUSH HL

    LD  HL, _ts_ids
    ADD HL, DE
    LD  A, (HL)
    LD  C, A        ; id

    ; select AY
    RLCA
    RLCA
    AND A, %11
    PUSH BC
    CALL snd_select_ay
    POP BC

    ; set mixer
    PUSH BC
    LD A, C
    AND A, %11
    NEG A
    DEC A
    LD D, A
    LD A, REG_MIXER
    CALL snd_get_ay_reg8
    AND A, D
    LD D, A
    LD A, REG_MIXER
    CALL snd_put_ay_reg8
    POP BC
    LD  D, 0
    
    ; write volume variable
    LD  HL, _ts_vol
    ADD HL, DE
    LD  A, 127
    LD (HL), A

    ; set volume register
    RLCA
    RLCA
    RLCA
    LD  D, A
    LD  A, C
    AND A, %11
    ADD A, 7
    PUSH BC
    CALL snd_put_ay_reg8
    POP BC

    ; set pitch register
    LD  A, C
    AND A, %10
    POP DE
    CALL snd_put_ay_reg16

__endasm;

}

void snd_update_tone_slot(ubyte idx)
{
    // ubyte id = ts_ids[idx];
    // ubyte ayn = id >> 6;
    // snd_select_ay(ayn);

    // sbyte vol = ts_vol[idx];
    // vol -= 3;

    // ubyte mixval = snd_get_ay_reg8(REG_MIXER);
    // if (vol < 0) {
    //     vol = 0;
    //     ubyte mask = id & 0b11;
    //     mixval |= mask;
    //     snd_put_ay_reg8(REG_MIXER, mixval);
    // }

    // ubyte chnum = 7 + (id & 0b11);
    // snd_put_ay_reg8(chnum, vol >> 3);
    // ts_vol[idx] = vol;

__asm
    LD  HL, 2
    ADD HL, SP
    LD  A, (HL)

    LD  D, 0
    LD  E, A        ; E <- idx

    LD  HL, _ts_ids
    ADD HL, DE
    LD  A, (HL)
    LD  C,  A       ; C <- id

    RLCA
    RLCA
    AND A, %11
    PUSH BC
    CALL snd_select_ay
    POP BC

    LD  HL, _ts_vol
    ADD HL, DE
    LD  A, (HL)     ; ts_vol[idx]

    ADD A, -3
    LD B, A
    AND A, $80
    JR Z, snd_update_tone_slot_ge0
    LD B, 0

    PUSH BC
    LD A, C
    AND A, %11
    LD D, A
    LD A, REG_MIXER
    CALL snd_get_ay_reg8
    OR A, D
    LD D, A
    LD A, REG_MIXER
    CALL snd_put_ay_reg8
    POP BC
    LD  D, 0

snd_update_tone_slot_ge0:
    LD A, B
    LD (HL), A

    RRCA
    RRCA
    RRCA
    LD  D, A
    LD  A, C
    AND A, %11
    ADD A, 7
    CALL snd_put_ay_reg8

__endasm

    // print_set_pos(10, 10+idx);
    // print_hex_byte(id);
    // print_char(' ');

    // print_hex_byte(ayn);
    // print_char(' ');

    // print_hex_byte(mixval);
    // print_char(' ');

    // print_hex_byte(prnum);
    // print_char(' ');

    // print_hex_byte(chnum);
    // print_char(' ');

    // print_hex_byte(vol);
    // print_char(' ');

}

void snd_init_crash_slot()
{
    // snd_select_ay(CRASH_AYN);

    // ubyte mixval = snd_get_ay_reg8(REG_MIXER);
    // mixval &= CRASH_NEG_MASK;
    // snd_put_ay_reg8(REG_MIXER, mixval);

    // // noise pitch
    // crash_pit = 0x18 << 3;
    // snd_put_ay_reg8(REG_NOISE_PITCH, crash_pit >> 3);

    // crash_vol = 127;
    // snd_put_ay_reg8(REG_VOL_C, crash_vol >> 3);
__asm
    LD A, CRASH_AYN
    CALL snd_select_ay
    
    LD A, REG_MIXER
    CALL snd_get_ay_reg8

    AND A, CRASH_NEG_MASK
    LD D, A
    LD A, REG_MIXER
    CALL snd_put_ay_reg8

    LD A, 192
    LD (_crash_pit), A
    RRCA
    RRCA
    RRCA
    LD D, A
    LD A, REG_NOISE_PITCH
    CALL snd_put_ay_reg8

    LD A, 127
    LD (_crash_vol), A
    RRCA
    RRCA
    RRCA
    LD D, A
    LD A, REG_VOL_C
    call snd_put_ay_reg8
__endasm
}

void snd_update_crash_slot()
{
    snd_select_ay(CRASH_AYN);

    crash_vol -= 6;

    ubyte mixval = snd_get_ay_reg8(REG_MIXER);
    if (crash_vol < 0) {
        crash_vol = 0;
        mixval |= CRASH_POS_MASK;
        snd_put_ay_reg8(REG_MIXER, mixval);
    }

    snd_put_ay_reg8(REG_VOL_C, crash_vol >> 3);

    crash_pit += 3;
    snd_put_ay_reg8(REG_NOISE_PITCH, crash_pit >> 3);
}

void snd_init_switch_slot()
{
    // snd_select_ay(SWITCH_AYN);

    // ubyte mixval = snd_get_ay_reg8(REG_MIXER);
    // mixval &= SWITCH_NEG_MASK;
    // snd_put_ay_reg8(REG_MIXER, mixval);

    // // noise pitch
    // snd_put_ay_reg8(REG_NOISE_PITCH, SWITCH_NOISE_PITCH);

    // // tone pitch
    // snd_put_ay_reg16(REG_PITCH_C, SWITCH_TONE_PITCH);

    // switch_vol = SWITCH_VOL;
    // snd_put_ay_reg8(REG_VOL_C, SWITCH_VOL);
__asm
    LD  A, SWITCH_AYN
    CALL snd_select_ay
    
    LD  A, REG_MIXER
    CALL snd_get_ay_reg8

    AND A, SWITCH_NEG_MASK
    LD  D, A
    LD  A, REG_MIXER
    CALL snd_put_ay_reg8

    LD  A, REG_NOISE_PITCH
    LD  D, SWITCH_NOISE_PITCH
    CALL snd_put_ay_reg8

    LD  A, REG_PITCH_C
    LD  DE, SWITCH_TONE_PITCH
    CALL snd_put_ay_reg16

    LD  A, SWITCH_VOL
    LD (_switch_vol), A
    LD  D, A
    LD  A, REG_VOL_C
    CALL snd_put_ay_reg8
__endasm
}

void snd_update_switch_slot()
{
    // snd_select_ay(SWITCH_AYN);

    // ubyte mixval = snd_get_ay_reg8(REG_MIXER);
    // mixval |= SWITCH_POS_MASK;
    // snd_put_ay_reg8(REG_MIXER, mixval);

    // switch_vol = 0;
__asm
    LD  A, SWITCH_AYN
    CALL snd_select_ay

    LD  A, REG_MIXER
    CALL snd_get_ay_reg8

    OR  A, SWITCH_POS_MASK
    LD  D, A
    LD  A, REG_MIXER
    CALL snd_put_ay_reg8

    LD A, 0
    LD (_switch_vol), A

    LD D, A
    LD A, REG_VOL_C
    CALL snd_put_ay_reg8
__endasm;
}













/*



__asm

_snd_setup:
	; Setup Turbo Sound chip
	LD BC, $FFFD		; Turbo Sound Next Control Register
	LD A, %11111101		; Enable left+right audio, select AY1
	OUT (C), A

	; Setup mapping of chip channels to stereo channels
	NEXTREG $08, %00010010	; Use ABC, enable internal speaker & turbosound
	NEXTREG $09, %11100000	; Enable mono for AY0-2

    RET

	; Play the tune
_snd_infinite_loop:
	LD HL, _snd_tune
	CALL _snd_play_tune	
	JP _snd_infinite_loop

	RET

_snd_curr_step:
    DEFW 0

_snd_init_tune:
    LD HL, _snd_tune
    LD (_snd_curr_step), HL

_snd_play_step:
    LD HL, (_snd_curr_step)
    CALL _snd_play_tune
    LD (_snd_curr_step), HL

; HL = address in memory where tune starts
_snd_play_tune:
	; Read noise/tone enable/disable
	LD A, (HL)	; Read next byte from (HL)
	CP $FF		; Is it $FF?
    JR NZ, _snd_play_tune_skip_reset
    ; it is $FF, reset to beginning
	LD HL, _snd_tune

_snd_play_tune_skip_reset:
	; We still have data, transfer A to D and write to AY register 7
	LD D, A
	LD A, 7
	INC HL
	CALL _snd_writeDToAYReg

	; Read tone period (regs 0 & 1)
	LD A, 0
	LD E, (HL)
	INC HL
	LD D, (HL)
	INC HL
	CALL _snd_writeDEToAYReg

	; Read noise period (reg 6)
	LD A, 6
	LD D, (HL)
	INC HL
	CALL _snd_writeDToAYReg

	; Read channel for setting up volume (0-2)
	LD A, (HL)
	INC HL
	ADD 8		; channel A is reg 8, B=9, C=10

	; Read and set volume to register in A
	LD D, (HL)
	INC HL
	CALL _snd_writeDToAYReg

    RET









	; Do a small pause between tones...
	LD BC, $FF00
_snd_delay:
	DEC BC
	LD A, B
	OR C
	JR NZ, _snd_delay

	; ...then continue with next tone
	JP _snd_play_tune

_snd_tune:
	;     Noise enable (0), disable (1)
	;     |  Tone enable (0), disable (1)
	;     |  |    Tone period ($0-$FFF)
	;     |  |    |        Noise period ($0-$1F)
	;     |  |    |        |    Channel (0=A, 1=B, 2=C)
	;     |--|--  |        |    |  Volume ($0-$F)
	;   --CBACBA  |------  |--  |  |-
	DB %00111110, $00,$3F, $00, 0, $F
	DB %00111110, $00,$3F, $00, 0, $F


	// DB %00111110, $00,$3F, $00, 0, $F
	// DB %00111110, $00,$97, $00, 0, $A
	// DB %00111110, $01,$00, $00, 0, $6
	// DB %00110110, $00,$00, $1F, 0, $4
	// DB %00110110, $00,$00, $11, 0, $4
	// DB %00111110, $00,$4C, $00, 0, $A
	// DB %00111110, $00,$7E, $00, 0, $7
	// DB %00110110, $00,$C9, $1F, 0, $4
	// DB %00110110, $01,$00, $11, 0, $4
	// DB %00111110, $00,$00, $00, 0, $2
	// DB %00111110, $00,$00, $00, 0, $2
	// DB %00111110, $01,$2E, $00, 0, $1 
	// DB %00111110, $01,$60, $00, 0, $1
	// DB %00111110, $01,$92, $00, 0, $1
	// DB %00111110, $00,$00, $00, 0, $2
	// DB %00111110, $03,$0F, $17, 0, $C
	// DB %00111110, $03,$C6, $10, 0, $B
	// DB %00111110, $04,$40, $0E, 0, $A
	// DB %00111110, $05,$35, $10, 0, $9
	// DB %00111110, $05,$EC, $13, 0, $8
	// DB %00111110, $06,$E1, $11, 0, $7
	DB $FF ; $FF = end of song

; Writes D to register A and E to A+1
; A = Starting register number
; DE = 16 bit value to write
_snd_writeDEToAYReg:
	PUSH AF		; _snd_writeDToAYReg will change A

	; Write D first
	CALL _snd_writeDToAYReg

	; Swap E to D and increment A, then continue straight to _snd_writeDToAYReg
	POP AF		; Restore A
	INC A		; Write to next register
	LD D, E		; Prepare value to write

; Note: register number (A) must have bit 7 reset, otherwise Next will use it as Turbo Sound Control! Should not be an issue as AY only uses registers 0-13
; A = Register number
; D = Value to write
_snd_writeDToAYReg:
	; Select desired register
	LD BC, $FFFD
	OUT (C), A

	; Write given value
	LD A, D
	LD BC, $BFFD
	OUT (C), A

	RET



__endasm;
*/

// The AY-3-8910 has 16 registers:
// Register  Purpose                   Width
// -------------------------------------------
// 0         Channel A fine pitch      8 bits
// 1         Channel A coarse pitch    4 bits
// 2         Channel B fine pitch      8 bits
// 3         Channel B coarse pitch    4 bits
// 4         Channel C fine pitch      8 bits
// 5         Channel C coarse pitch    4 bits
// 6         Noise pitch               5 bits
// 7         Mixer                     8 bits
// 8         Channel A volume          4 bits
// 9         Channel B volume          4 bits
// 10        Channel C volume          4 bits
// 11        Envelope fine duration    8 bits
// 12        Envelope coarse duration  8 bits
// 13        Envelope shape            4 bits
// 14        I/O port A                8 bits
// 15        I/O port B                8 bits