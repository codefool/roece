#pragma once
#include "roece.h"

class Square : public std::pair<byte,byte> {
public:
    Square();
    Square( byte rank, byte file );
    Square( const char *rnf );
    Square( const std::string rnf );

    inline const Rank rank() const { return Rank(first); }
    inline const File file() const { return File(second); }

    const short rank_delta( const Square& rhs ) const;
    const short file_delta( const Square& rhs ) const;
    const short rank_distance( const Square& rhs )  const;
    const short file_distance( const Square& rhs )  const;

    Offset delta( const Square& rhs ) const;

    bool in_bounds() const;
    bool is_unbounded() const;

    Square operator+=( const Offset& rhs );
    Square operator+( const Offset& rhs ) const;

    friend std::ostream& operator<<(std::ostream& os, const Square& squ);

public:
    static Square UNBOUNDED;
};

