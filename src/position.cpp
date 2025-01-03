#include "position.h"

GameInfoPacked::GameInfoPacked()
: i{0}
{}

GameInfoPacked::GameInfoPacked(uint32_t v)
: i{v}
{}

GameInfoPacked::GameInfoPacked(const GameInfoPacked& o)
: i{o.i}
{}

bool GameInfoPacked::operator==(const GameInfoPacked& o) const {
    bool ret = (i == o.i);
    return ret;
}

bool GameInfoPacked::operator!=(const GameInfoPacked& o) const {
    bool ret = (i != o.i);
    return ! (*this == o);
}

bool GameInfoPacked::operator<(const GameInfoPacked& o) const {
    bool ret = (i < o.i);
    return ret;
}

// When comparing GameInfoPacked values, we want to exclude the values
// of the clocks, because they are always changing and for the purposes
// of game play we aren't concerned with which move number in gameplay
// generated the position, only that the position was re-encountered.
//
// To deal with portability, we do the following monkey-love to 
// generate an appropriate mask at runtime.
//
uint32_t GameInfoPacked::ExcludeClockMask = [](){
    GameInfoPacked x(0xffffffff);
    x.f.full_move_cnt = 0;
    x.f.half_move_clock = 0;
    return x.i;
}();

uint32_t GameInfoPacked::packed_no_clocks() const {
    return i & ExcludeClockMask;
}

PositionPacked::PositionPacked() 
: gi(0), pop{0}
{
    pieces.w[0] = pieces.w[1] = 0;
}

PositionPacked::PositionPacked(const PositionPacked& o)
: gi(o.gi), pop{o.pop}
{
    pieces.w[0] = o.pieces.w[0];
    pieces.w[1] = o.pieces.w[1];
}

PositionPacked::PositionPacked(uint32_t g, uint64_t p, uint64_t h, uint64_t l)
: gi(g), pop{p}
{
    pieces.w[0] = h;
    pieces.w[1] = l;
}

std::string PositionPacked::hexString() const {
    int len((sizeof(PositionPacked)*2)+1);
    char b[len];
    char *p(b);
    const unsigned char *q((const unsigned char *)this);
    for ( int i = 0; i < sizeof(PositionPacked); ++i, p += 2, ++q ) {
        std::sprintf(p,"%02x", *q);
    }
    *p = '\0';
    return std::string(b);
}

bool PositionPacked::operator==(const PositionPacked& rhs) const {
    return memcmp((void *)this, (void *)&rhs, sizeof(PositionPacked)) == 0;
    // return pop == o.pop && !memcmp(pieces.w, o.pieces.w, sizeof(PositionPacked::pieces));
}

bool PositionPacked::operator!=(const PositionPacked& rhs) const {
    return ! (*this == rhs);
}

bool PositionPacked::compareNoClocks(const PositionPacked& rhs) const {
    return gi.packed_no_clocks() == rhs.gi.packed_no_clocks()
           && pop == rhs.pop
           && memcmp(pieces.w, rhs.pieces.w, sizeof(PositionPacked::pieces)) == 0;
}

bool PositionPacked::operator<(const PositionPacked& o) const {
    if ( pop == o.pop ) {
        if ( gi.i == o.gi.i ) {
            if ( pieces.w[0] == o.pieces.w[0] ) {
                return pieces.w[1] < o.pieces.w[1];
            }
            return pieces.w[0] < o.pieces.w[0];
        }
        return gi.i < o.gi.i;
    }
    return pop < o.pop;
}

std::ostream& operator<<(std::ostream& os, const PositionPacked& pp) {
	auto oflags = os.flags(std::ios::hex);
    auto ofill  = os.fill('0');
	auto owid   = os.width(8);

    os << pp.gi.i << ',';
    os.width(16);
    os << pp.pop << ','
       << pp.pieces.w[0] << ','
       << pp.pieces.w[1];

    os.width(2);
    for(int i{0}; i < 16; ++i) {
        os << ' ' << unsigned(pp.pieces.b[i].l) << ':' << unsigned(pp.pieces.b[i].r);
    }
    os << std::endl;

	os.flags(oflags);
	os.fill(ofill);
	os.width(owid);

	return os;
}

