#include "roece.h"

Square::Square()
{}

Square::Square( byte rank, byte file )
: pair(rank,file)
{}

Square::Square( uint8_t ord) 
: pair(ord / 8, ord % 8)
{}

// two character square id - file (a-h) rank(1-8), e.g. "a1", "e5", etc
Square::Square( const char *rnf ) {
    first  = rnf[1] - '1';               // rank is 1 origin
    second = std::tolower(rnf[0]) - 'a';
}

Square::Square( const std::string rnf )
: Square( rnf.c_str() )
{}

const short Square::rank_delta( const Square& rhs ) const {
    return rank() - rhs.rank();
}

const short Square::file_delta( const Square& rhs ) const {
    return file() - rhs.file();
}

const short Square::rank_distance( const Square& rhs ) const {
    return std::abs( rank_delta( rhs ) );
}

const short Square::file_distance( const Square& rhs ) const {
    return std::abs( file_delta( rhs ) );
}

Offset Square::delta( const Square& rhs ) const {
    return Offset(rank_delta(rhs), file_delta(rhs));
}

bool Square::in_bounds() const {
    return ( R1 <= rank() && rank() <= R8 && Fa <= file() && file() <= Fh );
}

bool Square::is_unbounded() const {
    return *this == UNBOUNDED;
}

Square Square::operator+=( const Offset& rhs ) {
    first  += rhs.first;
    second += rhs.second;
    return *this;
}

Square Square::operator+( const Offset& rhs ) const {
    return Square( first + rhs.first, second + rhs.second );
}

Square Square::UNBOUNDED(-1, -1);

std::ostream& operator<<(std::ostream& os, const Square& squ) {
    if ( squ == Square::UNBOUNDED ) {
        os << "--";
    } else {
        os << file_glyph(squ.file()) << rank_glyph(squ.rank());
    }
    return os;
}

byte Square::ordinal() const {
    return (uint64_t)(rank() * 8 + file());
}

