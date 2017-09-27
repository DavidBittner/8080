#include "emu.hh"

#include <sstream>
#include <iomanip>

#include <fmt/ostr.h>

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

template<class t>
std::string numToHex( t in )
{
    std::stringstream str;
    str << "0x" << std::setfill('0') << std::setw(sizeof(in)*2) << std::hex << static_cast<int>(in);
    return str.str().c_str();
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
        case inst::JMP:
        {
            skipbytes = 0;
            auto test = readArgs( 2 );

            //Take the address bytes and stick them together.
            //Each is a byte, we want a 16 bit address.
            uint16_t adr = test.at(1);
            adr = (adr<<8)|test.at(0);

            pc = adr;
            logger->log( spd::level::info, "Jumping to {}...", numToHex(adr) );

            break;
        }
        default:
        {
            std::string hex = numToHex(static_cast<uint8_t>(cur));
            logger->error( "Undefined instruction: '{}'", hex );
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

std::vector<uint8_t> Emulator_8080::readArgs( int count )
{
    std::vector<uint8_t> ret;
    for( int i = 1; i <= count; i++ )
    {
        ret.push_back(rom.get()[pc+i]);
    }
    std::string wrstr = "{} args received: [ ";
    for( auto i = ret.begin(); i != ret.end(); i++ )
    {
        if( i != ret.begin() )
        {
            wrstr.append(", ");
        }
        wrstr.append(std::to_string(*i));
    }
    wrstr.append( " ]" );

    logger->log( spd::level::info, wrstr.c_str(), count );
    return ret;
}
