#include "util.h"

const char *EMPTY_BOARD = nullptr;
const char *INITIAL_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// in is an octet array of len s
// out is an octet array of len s/2
// pack every two entries into one entry
void pack_array(uint8_t *in, uint8_t *out, size_t s)
{
    uint8_t *p = in;
    uint8_t *q = out;
    for( ; s; s -= 2, p += 2 )
    {
        *q++ = ((p[0] << 4) & 0xf0) | (p[1] & 0x0f);
    }
}

// in is an octet array of len s
// out is an octet array of len s*2
// upack every entry of in into two entries of out
void unpack_array(uint8_t* in, uint8_t* out, size_t s)
{
    uint8_t *p = in;
    uint8_t *q = out;
    for( ; s; s -= 2 )
    {
        *q++ = ((*p >> 4) & 0x0f );// high nibble
        *q++ = ( *p++     & 0x0f );// low nibble
    }
}

// hacker's delight (2nd ed) Fig 5-2
int bitpop64(std::uint64_t x) {
    x =  x - ((x >> 1) & 0x5555555555555555ull);
    x = (x & 0x3333333333333333ull) + ((x >> 2) & 0x3333333333333333ull);
    x = (x + (x >>  4)) & 0x0f0f0f0f0f0f0f0full;
    x =  x + (x >>  8);
    x =  x + (x >> 16);
    x =  x + (x >> 32);
    return (int)(x & 0x000000000000003Full);
}