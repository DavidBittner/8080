#ifndef EMU_HEADER
#define EMU_HEADER

#include <memory>
#include <vector>

#include <cstdint>

#include <spdlog.h>

#define MEMORY_SIZE 1024*1000

enum class inst;

class Emulator_8080
{
public:
    explicit Emulator_8080( std::shared_ptr<char> rom, int romlen );

    bool isActive();
    void step();

    int getpc() const;
    int getinst() const;

private:
    enum class conds;

    std::vector<uint8_t> readArgs( int count );

    bool get_flag( conds cond );
    void set_flag( conds cond, bool val );

    bool alive;

    int pc;
    uint16_t sp;

    int romlen;

    uint8_t *mem;
    uint8_t a, b, c, d, e, h, l;
    uint8_t status;

    inst cur;

    const std::shared_ptr<char> rom;
    std::shared_ptr<spdlog::logger> logger;

    //TODO: Add conditions (parity and such)
    enum class conds
    {
        SIGN   = 0,
        ZERO   = 1,
        AUX_C  = 3,
        PARITY = 5,
        CARRY  = 7,
    };

};

enum class inst
{
    NONE     = -0x01, //Just used for the inital state
    NOP      = 0x00,
    LXIBD16  = 0x01,
    STAXB    = 0x02,
    INXB     = 0x03,
    INRB     = 0x04,
    DCRB     = 0x05,
    MVIBD8   = 0x06,
    RLC      = 0x07,
    DADB     = 0x09,
    LDAXB    = 0x0a,
    DCXB     = 0x0b,
    INRC     = 0x0c,
    DCRC     = 0x0d,
    MVICD8   = 0x0e,
    RRC      = 0x0f,
    LXIDD16  = 0x11,
    STAXD    = 0x12,
    INXD     = 0x12,
    INRD     = 0x14,
    DCRD     = 0x15,
    MVIDD8   = 0x16,
    RAL      = 0x17,
    DADD     = 0x19,
    LDAXD    = 0x1a,
    DCXD     = 0x1b,
    INRE     = 0x1c,
    DCRE     = 0x1d,
    MVIED8   = 0x1e,
    RAR      = 0x1f,
    RIM      = 0x20,
    LXIHD16  = 0x21,
    LXISPD16 = 0x31,
    JMP      = 0xC3,
    ANID8    = 0xE6,
};

#endif
