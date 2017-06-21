#include "emu.h"

int op( struct emu *curEmu )
{
    unsigned char *opcode = &curEmu->mem[curEmu->pc];

    switch( *opcode )
    {
        case 0x00: break;
        default: unimpOp( curEmu ); break;
    }

    //From the opcode executing
    ++curEmu->pc;

    return -1;
}

void unimpOp( struct emu *curEmu )
{
    printf( "%04x unknown operation: 0x%02x\n", curEmu->pc, curEmu->mem[curEmu->pc] );
}
