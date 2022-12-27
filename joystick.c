#include "joystick.h"
#include "print_tile.h"

__asm

._joydata    defb 0

._joyUp      defb 0
._joyDown    defb 0
._joyLeft    defb 0
._joyRight   defb 0
._joyBut1    defb 0
._joyBut2    defb 0

__endasm

#define JOY_UP    0x08
#define JOY_DOWN  0x04
#define JOY_LEFT  0x02
#define JOY_RIGHT 0x01
#define JOY_BUT1  0x10
#define JOY_BUT2  0x20

void joystick_init()
{

}

void joystick_update()
{
__asm
    LD C, $1F
    IN A, (C)
    LD (_joydata), A

    LD HL, _joyUp
    LD B, 6
    XOR A
joystick_update_clear_loop:
    LD (HL), A
    INC HL
    DJNZ joystick_update_clear_loop

    LD A, (_joydata)
    LD B, A

    LD A, B
    AND JOY_UP
    JR Z, ASMPC+7   ; skip next two instructions
    LD A, 1
    LD (_joyUp), A

    LD A, B
    AND JOY_DOWN
    JR Z, ASMPC+7   ; skip next two instructions
    LD A, 1
    LD (_joyDown), A

    LD A, B
    AND JOY_LEFT
    JR Z, ASMPC+7   ; skip next two instructions
    LD A, 1
    LD (_joyLeft), A

    LD A, B
    AND JOY_RIGHT
    JR Z, ASMPC+7   ; skip next two instructions
    LD A, 1
    LD (_joyRight), A

    LD A, B
    AND JOY_BUT1
    JR Z, ASMPC+7   ; skip next two instructions
    LD A, 1
    LD (_joyBut1), A

    LD A, B
    AND JOY_BUT2
    JR Z, ASMPC+7   ; skip next two instructions
    LD A, 1
    LD (_joyBut2), A
__endasm

}

void joystick_print()
{
    print_set_pos(38, 31);
    print_set_color(CTEXT_WHI);
    print_hex_byte(joydata);
}