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
        *q++ = ((*p   & 0xf0 ) >> 4);
        *q++ = ( *p++ & 0x0f );
    }
}
