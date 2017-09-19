#include <iostream>
#include <fstream>

#include <memory>

#include <spdlog.h>

#include "emu.hh"

namespace spd = spdlog;
int main( int argc, char **argv )
{
    auto console = spd::stdout_color_mt("main");
    console->log( spd::level::info, "Initializing...");

    if( argc <= 1 )
    {
        console->error( "No ROM supplied. Exiting." );
        return -1;
    }
    
    console->log( spd::level::info, "Opening file: "+std::string(argv[1]));
    std::ifstream file( std::string(argv[1]), std::ios::binary | std::ios::ate | std::ios::in );

    if( !file.good() )
    {
        console->error( "Invalid file: '"+std::string(argv[1])+"'. Empty or not found. Terminating." );
    }else{
        console->log( spd::level::info, "Reading from file: '" + std::string(argv[1]) + "'..." );
    }

    long size = file.tellg();
    std::shared_ptr<char> rom(new char[size]);

    file.seekg(0, std::ios::beg);
    file.read(rom.get(), size);

    console->log( spd::level::info, "Read {0:.2f} kilo-bytes into instruction memory...", size/1000.0 ); 

    //Starting emulation
    Emulator_8080 emu(rom, size);

    while( emu.isActive() )
    {
        emu.step();
    }

    return 1;
}
