#ifndef EMU_HEADER
#define EMU_HEADER

#include <memory>
#include <vector>

#include <spdlog.h>

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
    std::vector<int> readArgs( int count );

    bool alive;

    int pc;
    int romlen;

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
};

#endif
