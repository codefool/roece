#include "roece.h"

Piece::Piece( PieceType pt, Board *b, Color c )
: _t(pt), _b(b), _c(c)
{
    set_glyph();
}

      Board&    Piece::board()  const { return *_b; }
const PieceType Piece::type()   const { return _t; }
const Color     Piece::color()  const { return _c; }
const Square    Piece::square() const { return _s; }
const char      Piece::glyph()  const { return _g; }
const byte      Piece::range()  const { return ranges[_t]; }

const bool Piece::is_empty()  const { return _t == PT_EMPTY; }
const bool Piece::is_knight() const { return _t == PT_KNIGHT; }
const bool Piece::is_white()  const { return _c == WHITE; }
const bool Piece::is_black()  const { return _c == BLACK; }

const bool Piece::is_enemy( PiecePtr ptr ) const {
    return color() != ptr->color();
}
const bool Piece::is_friend( PiecePtr ptr ) const {
    return ! is_enemy( ptr );
}

void Piece::set_square(Square squ) {
    _s = squ;
}

void Piece::set_glyph() {
    _g = glyphs[type()];
    if (is_black())
        _g = std::tolower(_g);
}

void Piece::set_board(Board* brd) {
    _b = brd;
}

const DirList& Piece::get_dirs() const { return none_dirs; }
bool Piece::move( Square dst ) { return false; }
bool Piece::promote( PieceType pt ) { return false; }

PieceList Piece::get_attackers(PiecePtr trg) {
    PieceList enemies;
    SeekResult sr;

    if ( trg->is_empty() )
        return enemies;

    // using trg as a starting point, trace all rays
    // axes, diag, and knight to see if an opposing
    // piece of the appropriate type is encountered.
    for ( Dir dir : omni_dirs ) {
        sr = board().seek( trg, dir, 7 );
        if ( sr.rc == SEEKRC_FOUND_ENEMY )
            enemies.push_back( sr.occupant );
    }
    for ( Dir dir : knight_dirs ) {
        PiecePtr enemy = board().at( trg->square() + offs[ dir ] );
        if ( enemy->is_knight() && is_enemy( enemy ) )
            enemies.push_back( enemy );
    }

    for( auto itr = enemies.begin(); itr != enemies.end(); ) {
        if ( (*itr)->can_attack(trg->square()) )
            ++itr;
        else
            itr = enemies.erase(itr);
    }

    return enemies;
}

bool Piece::can_diag_attack( Square dst ) const {
    return square().rank_dist( dst ) == square().file_dist( dst );
}

bool Piece::can_axes_attack( Square dst ) const {
    return square().rank_dist( dst ) == 0 || square().file_dist( dst ) == 0;
}

bool Piece::can_omni_attack( Square dst ) const {
    return can_diag_attack(dst) || can_axes_attack(dst);
}

void Piece::get_dirs_moves( const DirList& dirs, MoveList& moves ) const {
    SeekResult res;
    PiecePtr ptr = board().at( square() );
    for ( Dir dir : dirs ) {
        res = board().seek( ptr, dir, range() );
        short sz( res.path.size() );
        for ( short idx(0); idx < sz; ++idx ) {
            MoveAction ma  = MV_MOVE;
            Square     squ = res.path[idx];
            if ( idx == sz - 1 && res.occupant != nullptr && !res.occupant->is_empty() ) {
                // the last square of the walk is not empty,
                // if it's friendly we're done. If not, capture it.
                if ( res.rc == SEEKRC_FOUND_FRIENDLY )
                    break;
                // mark last move as capture
                ma = MV_CAPTURE;
            }
            moves.push_back( Move( ma, square(), squ ) );
        }
    }
}

void Piece::get_diag_moves( MoveList& moves ) const {
    get_dirs_moves( diag_dirs, moves );
}

void Piece::get_axes_moves(MoveList& moves) const {
    get_dirs_moves( axes_dirs, moves );
}

void Piece::get_omni_moves(MoveList& moves) const {
    get_diag_moves( moves );
    get_axes_moves( moves );
}

PiecePtr Piece::factory(PieceType pt, Board* b, Color c ) {
    switch(pt) {
    case PT_KING:   return std::make_shared<King>  (c, b);
    case PT_QUEEN:  return std::make_shared<Queen> (c, b);
    case PT_BISHOP: return std::make_shared<Bishop>(c, b);
    case PT_KNIGHT: return std::make_shared<Knight>(c, b);
    case PT_ROOK:   return std::make_shared<Rook>  (c, b);
    case PT_PAWN:   return std::make_shared<Pawn>  (c, b);
    }
    return nullptr;
}

Empty::Empty()
: Piece( PT_EMPTY, nullptr, NONE )
{}

bool Empty::can_attack( Square dst ) const {
    return false;
}
void Empty::get_moves( MoveList& moves ) const { 
}

void Empty::apply_move( const Move& move )
{}


PiecePtr Piece::EMPTY = std::make_shared<Empty>();
const char *Piece::glyphs=".KWBNRPP";
// range by PieceType ordinal
const byte Piece::ranges[8] = {1,1,7,7,1,7,0,0};

King::King(Color c, Board* b)
: Piece(PT_KING, b, c)
{}

const DirList& King::get_dirs() const {
    return omni_dirs;
}
void King::get_moves( MoveList& moves ) const { 
    get_omni_moves( moves ); 
    // check castle moves
}
bool King::can_attack( Square dst ) const {
    return can_omni_attack(dst);
}
void King::apply_move( const Move& move )
{}

Queen::Queen(Color c, Board* b)
: Piece(PT_QUEEN, b, c)
{}

const DirList& Queen::get_dirs() const {
    return omni_dirs;
}
void Queen::get_moves( MoveList& moves ) const { 
    get_omni_moves( moves ); 
}
bool Queen::can_attack( Square dst ) const {
    return can_omni_attack(dst);
}
void Queen::apply_move( const Move& move )
{}


Bishop::Bishop(Color c, Board* b)
: Piece(PT_BISHOP, b, c)
{}

const DirList& Bishop::get_dirs() const {
    return diag_dirs;
}
void Bishop::get_moves( MoveList& moves ) const { 
    get_diag_moves( moves ); 
}
bool Bishop::can_attack( Square dst ) const {
    return can_diag_attack(dst);
}
void Bishop::apply_move( const Move& move )
{}

Knight::Knight(Color c, Board* b)
: Piece(PT_KNIGHT, b, c)
{}

const DirList& Knight::get_dirs() const {
    return knight_dirs;
}

void Knight::get_moves( MoveList& moves ) const {
    get_dirs_moves( knight_dirs, moves );
}

bool Knight::can_attack( Square dst ) const {
    byte dr = square().rank_dist(dst);
    byte df = square().file_dist(dst);
    return ( dr == 2 && df == 1 ) || ( dr == 1 && df == 2 );
}
void Knight::apply_move( const Move& move )
{}



Rook::Rook(Color c, Board* b)
: Piece(PT_ROOK, b, c)
{}

const DirList& Rook::get_dirs() const {
    return axes_dirs;
}
void Rook::get_moves( MoveList& moves ) const { 
    get_axes_moves( moves ); 
}
bool Rook::can_attack( Square dst ) const {
    return can_axes_attack(dst);
}
void Rook::apply_move( const Move& move )
{}



Pawn::Pawn(Color c, Board* b)
: Piece(PT_PAWN, b, c)
{}

const DirList& Pawn::get_dirs() const {
    return none_dirs;
}

void Pawn::get_moves( MoveList& moves ) const { 
    Rank   rank((is_black())?R7:R2);
    Dir    dir ((is_black())?DN:UP);
    Square org (square());
    Square dst( org + offs[dir] );
    if ( dst.in_bounds() ) {
        PiecePtr ptr( board().at(dst) );
        if ( ptr->is_empty() )
        moves.push_back( Move( MV_MOVE, org, dst ) );
        if ( org.rank() == rank ) {
            // pawn is still on home file, so can move two places
            dst += offs[ dir ];
            PiecePtr ptr( board().at(dst) );
            if ( ptr->is_empty() )
                moves.push_back( Move( MV_MOVE, org, dst ) );
        }
    }
    // check for captures
    const DirList& dirs( (is_black()) ? pawn_black : pawn_white );
    for ( Dir dir : dirs ) {
        Square dst( org + offs[ dir ] );
        if ( !dst.in_bounds() )
            continue;
        PiecePtr ptr( board().at(dst));
        if ( ptr->is_empty() )
            continue;
        moves.push_back( Move( MV_CAPTURE, org, dst ) );
    }

    // check for en passant
    if ( board().has_en_passant() ) {
        Square   dst( board().get_en_passant() );
        PiecePtr trg( board().at(dst) );
        if ( is_enemy(trg) ) {
            Rank     rank( (is_black()) ? R4 : R5 );
            Dir      dir ( (is_black()) ? DN : UP );
            if ( org.rank() == rank && org.file_dist(dst) == 1 ) {
                dst += offs[ dir ];
                moves.push_back( Move( MV_EN_PASSANT, org, dst ) );
            }
        }
    }
}

bool Pawn::can_attack( Square dst ) const {
    bool ret(false);
    byte dr( square().rank_dist( dst ) );
    byte df( square().file_dist( dst ) );
    // TODO: if dst == board().en_passant()
    if ( is_white() ) {
        // white pawn attack up, so dr must be -1 and df is -/+1
        ret = dr == -1 && df == 1;
    } else {
        // black pawn attack down, so dr must be 1 and df is -/+1
        ret = dr == 1 && df == 1;
    }
    return ret;
}

void Pawn::apply_move( const Move& move )
{}
