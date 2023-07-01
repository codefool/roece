#include <cassert>
#include "roece.h"

Piece::Piece()
: _t(PT_NONE), _b(nullptr), _c(NONE), _s(Square::UNBOUNDED)
{}
King::King() : Piece()
{}
Queen::Queen() : Piece()
{}
Bishop::Bishop() : Piece()
{}
Knight::Knight() : Piece()
{}
Rook::Rook() : Piece()
{}
Pawn::Pawn() : Piece()
{}

Piece::Piece( PieceType pt, Board *b, Color c )
: _t(pt), _b(b), _c(c), _s(Square::UNBOUNDED)
{
    set_glyph();
}

Board& Piece::board() const { 
    assert(_b != nullptr);
    return *_b; 
}

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

PiecePtr Piece::ptr() {
    return board().at(square());
}

void Piece::set_type(PieceType pt) {
    _t = pt;
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

// move the piece as indicated in the move, and handles any capture.
// Assumptions:
// 1. The move is legally valid
// 2. Any special handling (castling, en passant, etc) have been
//    taken care of by sub-classes.
MoveAction Piece::move( const Move move ) {
    board().set(move.dst, ptr());
    return move.action; 
}

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
    return square().rank_distance( dst ) == square().file_distance( dst );
}

bool Piece::can_axes_attack( Square dst ) const {
    return square().rank_distance( dst ) == 0 || square().file_distance( dst ) == 0;
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

// the common piece that occupies every empty square.
// its color is NONE
PiecePtr Piece::EMPTY = std::make_shared<Empty>();

const char *Piece::glyphs=".KQBNRPP";
// range by PieceType ordinal  W K Q B N E P P
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
MoveAction King::move(const Move move) {
    PiecePtr trg = board().at(move.dst);
    if (move.action == MV_CASTLE_KINGSIDE || move.action == MV_CASTLE_QUEENSIDE ) {
        CastleColor cc = (is_black()) ? CASTLE_BLACK : CASTLE_WHITE;
        CastleSide  cs = (move.action == MV_CASTLE_KINGSIDE) 
                       ? CASTLE_KINGSIDE 
                       : CASTLE_QUEENSIDE;

        // in this case, the src is the king and the trg is the rook being
        // castled. 
        File king = (cs == CASTLE_KINGSIDE) ? Fg : Fb;
        File rook = (cs == CASTLE_KINGSIDE) ? Ff : Fc;

        board().set(Square(square().rank(),      king), ptr());
        board().set(Square(trg->square().rank(), rook), trg);
        board().set_castle_right(cc, cs, false);

        return move.action;
    } 
    return Piece::move(move);
}

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
    byte dr = square().rank_distance(dst);
    byte df = square().file_distance(dst);
    return ( dr == 2 && df == 1 ) || ( dr == 1 && df == 2 );
}


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


Pawn::Pawn(Color c, Board* b)
: Piece(PT_PAWN, b, c)
{}

const DirList& Pawn::get_dirs() const {
    return none_dirs;
}

void Pawn::get_moves( MoveList& moves ) const { 
    Rank     prom( is_black() ? R1 : R8 );  // rank where pawn is promoted
    Rank     home( is_black() ? R7 : R2 );  // rank where pawn starts
    Dir      dir ( is_black() ? DN : UP );  // pawns can only up forward (unless capturing)
    Square   org (square());
    Square   dst( org + offs[dir] );
    PiecePtr trg;

    if ( dst.in_bounds() ) {
        if ( board().at(dst)->is_empty() ) {
            moves.push_back( Move( MV_MOVE, org, dst ) );
            if ( org.rank() == home ) {
                // pawn is still on home rank, so can move two places
                dst += offs[ dir ];
                if ( board().at(dst)->is_empty() )
                    moves.push_back( Move( MV_MOVE, org, dst ) );
            } else if (dst.rank() == prom ) {
                // pawn is being promoted - push a move for each possible type
                for ( short ma = MV_PROM_QUEEN; ma <= MV_PROM_ROOK; ++ma)
                    moves.push_back( Move( MoveAction(ma), org, dst ) );
            }
        }
    }

    // check for captures
    const DirList& dirs( (is_black()) ? pawn_black : pawn_white );
    for ( Dir dir : dirs ) {
        Square dst( org + offs[ dir ] );
        if ( !dst.in_bounds() )
            continue;
        trg = board().at(dst);
        if ( trg->is_empty() || is_friend(trg) )
            continue;
        if ( dst.rank() == prom ) {
            // the capture advances pawn to promotion rank
            for ( uint8_t ma = MV_CAP_PROM_QUEEN; ma <= MV_CAP_PROM_ROOK; ++ma )
                moves.push_back( Move( MoveAction(ma), org, dst ) );
        } else {
            moves.push_back( Move( MV_CAPTURE, org, dst ) );
        }
    }

    // check for en passant
    if ( board().has_en_passant() ) {
        dst = board().get_en_passant();
        if ( is_enemy( board().at( dst ) ) ) {
            Rank rank( is_black() ? R4 : R5 );
            if ( org.rank() == rank && org.file_distance(dst) == 1 ) {
                dst += offs[ dir ];
                moves.push_back( Move( MV_EN_PASSANT, org, dst ) );
            }
        }
    }
}

bool Pawn::can_attack( Square dst ) const {
    bool ret(false);
    byte dr( square().rank_delta( dst ) );
    byte df( square().file_distance( dst ) );
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

MoveAction Pawn::move(const Move move) {
    if ( move.action == MV_EN_PASSANT ) {
        PiecePtr capt = board().at(move.org.rank(), move.dst.file());
        if ( capt->is_empty() )
            return MV_NONE;
        board().remove(capt);
        return Piece::move(move);
    } else if ( move.action == MV_MOVE ) {
        // standard move - but pawn can move 1 or 2 spaces.
        // if 2 spaces set en passant square
        Piece::move(move);
        if ( move.org.rank_distance(move.dst) == 2 )
            board().set_en_passant(move.dst);
    } else {
        Piece::move(move);
        PieceType pt(PT_NONE);
        switch(move.action) {
	    case MV_PROM_QUEEN : case MV_CAP_PROM_QUEEN : pt = PT_QUEEN;  break;
	    case MV_PROM_BISHOP: case MV_CAP_PROM_BISHOP: pt = PT_BISHOP; break;
	    case MV_PROM_KNIGHT: case MV_CAP_PROM_KNIGHT: pt = PT_KNIGHT; break;
	    case MV_PROM_ROOK  : case MV_CAP_PROM_ROOK  : pt = PT_ROOK;   break;
        }
        if ( pt != PT_NONE )
            set_type(pt);
    }
    // pawn moved. Clear half-move clock
    board().clear_half_move_clock();
    return move.action;
}

