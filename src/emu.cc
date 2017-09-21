#include "emu.hh"

#include <sstream>
#include <iomanip>

namespace spd = spdlog;

Emulator_8080::Emulator_8080( std::shared_ptr<char> rom, int romlen ) :
    rom(rom),
    logger(spd::stdout_color_mt("emu"))
{
    this->romlen = romlen;
    this->pc = 0;
    this->alive = true;

    cur = inst::NONE;

    logger->log( spd::level::info, "Initialized emulator." );
}

void Emulator_8080::step()
{
    if( pc == romlen )
    {
        logger->log( spd::level::info, "Reached end of executable, emulator dead." );
        alive = false;
        return;
    }

    //If jumped, we don't want to increment the program counter at the end
    int skipbytes = 0;

    unsigned char holder = rom.get()[pc];
    cur = static_cast<inst>(holder);

    switch( cur )
    {
        case inst::NONE:
        { 
            skipbytes = 1;
            break;
        }
        case inst::NOP:
        {
            skipbytes = 1;
            break;
        }
        default:
        {
            //Ew. I couldn't figure out how to do this with spd. I'm sorry :c
            std::stringstream str;
            str << "0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(cur);

            logger->error( "Undefined instruction: '{}'", str.str() );
            alive = false;
            return;
        }
    }
    pc += skipbytes;
}

bool Emulator_8080::isActive()
{
    return alive;
}
