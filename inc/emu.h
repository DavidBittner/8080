#ifndef EMU_HEADER
#define EMU_HEADER

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct emu 
{
    bool haltflag;

    uint8_t *mem;
    int memstart;
    
    uint16_t *sp;
    uint16_t pc;

    //Registers
    uint8_t a, b, c, d, e, h, l;

    //ConditionCodes
    uint8_t z:1, s:1, p:1, cy:1, ac:1, pad:3;

    //Enable interrupts?
    uint8_t int_enable;
};

//TODO: Returns cycles command takes for accurate timing
int op( struct emu *curEmu );

void unimpOp( struct emu *curEmu );

#endif
