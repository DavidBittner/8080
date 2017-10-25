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
    pc = 0;
    sp = 0;
    alive = true;

    this->status = 0;

    cur = inst::NONE;

    a = 0; b = 0; c = 0; d = 0; e = 0; h = 0; l = 0;

    this->mem = new uint8_t[16000];

    logger->log( spd::level::info, "Initialized emulator." );
}

template<class t>
std::string numToHex( t in )
{
    std::stringstream str;
    str << "0x" << std::setfill('0') << std::setw(sizeof(in)*2) << std::hex << static_cast<int>(in);
    return str.str();
}

bool parity( uint8_t val )
{

    bool parity = false;
    while( val )
    {
        parity = !parity;
        val = val & (val - 1);
    }

    return parity;
}

bool Emulator_8080::get_flag( conds cond )
{
    uint8_t temp_status = this->status;
    int lshift = 8-static_cast<int>(cond);
    int rshift = static_cast<int>(cond);

    temp_status = (temp_status << lshift) >> lshift;
    temp_status = (temp_status >> rshift );

    return temp_status;
}

void Emulator_8080::set_flag( conds cond, bool val  )
{
    if( val )
    {
        uint8_t setter = 1 << static_cast<int>(cond);
        this->status = status | setter;
    }else{
        uint8_t setter = ~(1 << static_cast<int>(cond));
        this->status = status & setter;
    }
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
    int  skipbytes = 0;
    bool updateFlags = false;
    int  lastOp = -1;

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
        case inst::LXIBD16: 
        {
            auto args = read_args( 2 );

            this->b = args.at(1);
            this->c = args.at(0);

            skipbytes = 3;
            logger->log( spd::level::info, "LXI B" );

            break;
        }
        case inst::MVIBD8:
        {
            auto args = read_args( 1 );

            uint8_t immediate = rom.get()[pc+1];
            this->b = immediate;

            skipbytes = 2;
            logger->log( spd::level::info, "MVI B [{}]", immediate );
        }
        case inst::LXISPD16:
        {
            auto args = read_args( 2 );

            uint16_t tmp = args.at(1);
            tmp = (tmp<<8)|args.at(0);

            this->sp = tmp;

            skipbytes = 3;
            logger->log( spd::level::info, "LXI SP [{}]", tmp );

            break;
        }
        case inst::JMP:
        {
            skipbytes = 0;
            auto args = read_args( 2 );

            //Take the address bytes and stick them together.
            //Each is a byte, we want a 16 bit address.
            uint16_t adr = args.at(1);
            adr = (adr<<8)|args.at(0);

            pc = adr;
            logger->log( spd::level::info, "JMP {}", numToHex(adr) );

            break;
        }
        case inst::ANID8:
        {
            skipbytes = 2;
            auto args = read_args( 1 );

            logger->log( spd::level::info, "ANI {} {}", a, args.at(0));
            this->a = a & args.at(0);
            lastOp = this->a;

            updateFlags = true;

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

    if( updateFlags )
    {
        if( lastOp >= 0 ){
            set_flag( conds::SIGN, true );
        }else {
            set_flag( conds::SIGN, false );
        }

        if( lastOp == 0 ) {
            set_flag( conds::ZERO, true );
        }else{
            set_flag( conds::ZERO, false );
        }

        set_flag( conds::PARITY, parity(lastOp) );
    }

    print_status();
}

bool Emulator_8080::is_active()
{
    return alive;
}

std::vector<uint8_t> Emulator_8080::read_args( int count )
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

void Emulator_8080::print_status()
{
    logger->log( spd::level::info, "REGS: A:{} B:{} C:{} D:{} E:{} H:{} L:{}", a, b, c, d, e, h, l );
    logger->log( spd::level::info, "FLAG: SIGN:{} ZERO:{} AUXC:{} PARI:{} CARRY:{}", 
                 get_flag(conds::SIGN), 
                 get_flag(conds::ZERO),
                 get_flag(conds::AUX_C),
                 get_flag(conds::PARITY),
                 get_flag(conds::CARRY) );

    logger->log( spd::level::info, "PC:{} SP:{}", pc, sp );
}