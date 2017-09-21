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
    std::vector<uint8_t> readArgs( int count );

    bool alive;

    int pc;
    int romlen;

    char *mem;
    uint8_t a, b, c, d, e, h, l;

    inst cur;

    const std::shared_ptr<char> rom;
    std::shared_ptr<spdlog::logger> logger;
};

enum class inst
{
    NONE    = -0x01, //Just used for the inital state
    NOP     = 0x00,
    LXIBD16 = 0x01,
    STAXB   = 0x02,
    INXB    = 0x03,
    INRB    = 0x04,
    JMP     = 0xC3
};

#endif
