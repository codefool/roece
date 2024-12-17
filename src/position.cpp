#include "position.h"

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

PositionPacked::PositionPacked() 
: gi(0), pop{0}, hi{0}, lo{0}
{}

PositionPacked::PositionPacked(const PositionPacked& o)
: gi(o.gi), pop{o.pop}, hi{o.hi}, lo{o.lo}
{}

PositionPacked::PositionPacked(uint32_t g, uint64_t p, uint64_t h, uint64_t l)
: gi(g), pop{p}, hi{h}, lo{l}
{}

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


bool PositionPacked::operator==(const PositionPacked& o) const {
    return pop == o.pop && gi == o.gi && hi == o.hi && lo == o.lo;
}

bool PositionPacked::operator!=(const PositionPacked& o) const {
    return ! (*this == o);
}

bool PositionPacked::operator<(const PositionPacked& o) const {
    if ( pop == o.pop ) {
        if ( gi.i == o.gi.i ) {
            if ( hi == o.hi ) {
                return lo < o.lo;
            }
            return hi < o.hi;
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
       << pp.hi << ','
       << pp.lo;

	os.flags(oflags);
	os.fill(ofill);
	os.width(owid);
	return os;
}

