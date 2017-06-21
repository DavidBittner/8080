#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "emu.h"

#define CLOCK_SPEED 2 * 1000000

int main( int argc, char **argv )
{

    //If no arg was given, quit
    if( argc <= 1 )
    {
        printf("No file supplied\n");
        exit(1);
    }

    //Open file given on command line
    FILE *rd = fopen(argv[1], "rb");
    if( !rd )
    {
        printf("Could not open %s\n", argv[1]);
        exit(2);
    }

    fseek( rd, 0l, SEEK_END );
    long sz = ftell( rd )*1000;
    fseek( rd, 0l, SEEK_SET );

    uint8_t ROM[sz];

    //Load the rom into memory, storing it in ROM
    for( long fc = 0; fc < sz; fc++ )
    {
        ROM[fc] = fgetc(rd);
    }
    //Close file
    fclose(rd);

    //Create emulator instance
    struct emu emulator;

    //Put stack
    //emulator.sp = &ROM[0];
    emulator.mem = ROM;
    emulator.haltflag = false;

    while( !emulator.haltflag )
    {
        op( &emulator );
        getchar();
    }

    return 0;
}
