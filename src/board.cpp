#include "roece.h"

std::vector<std::string> split(std::string target, std::string delimiter) {
    std::vector<std::string> v;

    if (!target.empty()) {
        std::string::size_type start = 0;
        while(true) {
            size_t x = target.find(delimiter, start);
            if (x == std::string::npos)
                break;

            v.push_back(target.substr(start, x-start));
            start = x + delimiter.size();
        }

        v.push_back(target.substr(start));
    }
    return v;
}

Board::Board(bool init) {
    if ( init )
        from_fen(initial_position_fen);
}

Board Board::deep_copy() const
{
    Board cpy(false);
    cpy.set_on_move( get_on_move() );
    cpy.set_en_passant( get_en_passant() );
    cpy.set_half_move_clock( get_half_move_clock() );
    cpy.set_full_move_count( get_full_move_count() );
    for ( short idx(0); idx < 4; ++idx ) 
        cpy.set_castle_right( idx, get_castle_right(idx) );
    for ( auto entry : _pm ) {
        const Square squ( entry.first );
        PiecePtr     o_ptr( entry.second );
        PiecePtr     ptr( Piece::factory( o_ptr->type(), &cpy, o_ptr->side() ));
        ptr->set_square(squ);
        cpy.set( squ, ptr );
    }
    return cpy;
}

void Board::apply_move(const Move& move) {
    PiecePtr ptr( at(move.org));
    ptr->apply_move(move);
}

Side Board::get_on_move() const {
    return _on_move;
}

void Board::set_on_move(Side s) {
    _on_move = s;
}

bool Board::none_can_castle() const {
    return !_castle_rights[0] && !_castle_rights[1] && !_castle_rights[2] && !_castle_rights[3];
}

void Board::clear_castle_rights() {
    std::memset(_castle_rights, 0x00, sizeof(_castle_rights));
}

bool Board::get_castle_right( byte idx ) const {
    return _castle_rights[ idx ];
}

void Board::set_castle_right( byte idx, bool state ) {
    _castle_rights[ idx ] = state;
}

bool Board::has_en_passant() const {
    return _en_passant != Square::UNBOUNDED;
}

Square Board::get_en_passant() const {
    return _en_passant;
}

void Board::clear_en_passant() {
    _en_passant = Square::UNBOUNDED;
}

void Board::set_en_passant( Square squ ) {
    _en_passant = squ;
}

short Board::get_half_move_clock() const {
    return _half_move_clock;
}

void Board::set_half_move_clock( short val ) {
    _half_move_clock = val;
}

void Board::clear_half_move_clock() {
    _half_move_clock = 0;
}

void Board::inc_half_move_clock() {
    ++_half_move_clock;
}

short Board::get_full_move_count() const {
    return _full_move_cnt;
}

void Board::set_full_move_count( short val ) {
    _full_move_cnt = val;
}

void Board::clear_full_move_count() {
    _full_move_cnt = 0;
}

void Board::inc_full_move_count() {
    _full_move_cnt++;
}

SeekResult Board::seek( PiecePtr src, Dir dir, short range ) {
    SeekResult ret;
    Square here = src->square();
    while ( range-- ) {
        here += offs[ dir ];
        if ( !here.in_bounds() )
            break;
        ret.path.push_back(here);
        PiecePtr occ = at(here);
        if ( ! occ->is_empty() ) {
            ret.occupant = occ;
            ret.rc = (src->is_enemy(occ)) 
                    ? SEEKRC_FOUND_ENEMY 
                    : SEEKRC_FOUND_FRIENDLY;
            break;
        }
    }
    return ret;
}

PiecePtr Board::at(Square squ) {
    auto itr = _pm.find(squ);
    if ( itr == _pm.end() )
        return Piece::EMPTY;
    return itr->second;
}

PiecePtr Board::make_piece( PieceType pt, Side s ) {
    return Piece::factory(pt, this, s);
}

void Board::set( Square squ, PiecePtr ptr ) {
    _pm[squ] = ptr;
    ptr->set_square(squ);
}

// Initialize a board from Forsyth-Edwards (FEN) notation string.
//
// 
void Board::from_fen(const std::string& fen)
{
    _pm.clear();

    std::vector<std::string> toks = split(fen, " ");
    // Field 1 - Piece Placement Data
    //
    // - Each rank is described, starting with rank 8 and ending with rank 1,
    //   with a "/" between each one; within each rank, the contents of the 
    //   squares are described in order from the a-file to the h-file.
    // - Each piece is identified by a single letter taken from the standard
    //   English names in algebraic notation (pawn = "P", knight = "N",
    //   bishop = "B", rook = "R", queen = "Q" and king = "K").
    // - White pieces are designated using uppercase letters ("PNBRQK"), while
    //   black pieces use lowercase letters ("pnbrqk").
    // - A set of one or more consecutive empty squares within a rank is denoted
    //   by a digit from "1" to "8", corresponding to the number of squares.
    //
    short rank(R8);
    short file(Fa);
    for (const char *p = toks[0].c_str(); *p; ++p ) {
        if ( std::isalpha(*p) ) {
            // piece type
            PieceType pt;
            Side      side = (std::islower(*p)?SIDE_BLACK:SIDE_WHITE);
            switch(std::toupper(*p)) {
                case 'K': pt = PT_KING;   break;
                case 'Q': pt = PT_QUEEN;  break;
                case 'B': pt = PT_BISHOP; break;
                case 'N': pt = PT_KNIGHT; break;
                case 'R': pt = PT_ROOK;   break;
                case 'P': pt = PT_PAWN;   break;
            }
            set(Square(rank, file++), Piece::factory( pt, this, side) );
        } else if ( std::isdigit(*p) ) {
            // count of empty squares
            file += short( *p - '0' );
        } else if ( *p == '/' ) {
            // end of rank
            --rank;
            file=Fa;
        } else {
            // error! unknown char
        }
    }

    // Field 2 - Active Color
    // - "w" means that White is to move; "b" means that Black is to move
    //
    set_on_move(( std::tolower(toks[1].at(0)) == 'b' ) ? SIDE_BLACK : SIDE_WHITE);

    // Field 3 - Castling Availability
    // - If neither side has the ability to castle, this field uses the 
    //   character "-". 
    // - Otherwise, this field contains one or more letters: 
    //   - "K" if White can castle kingside, 
    //   - "Q" if White can castle queenside, 
    //   - "k" if Black can castle kingside, and 
    //   - "q" if Black can castle queenside. 
    // - A situation that temporarily prevents castling does not prevent
    //    the use of this notation.
    //
    clear_castle_rights();
    if ( toks[2] != "-" ) {
        for( const char *p = toks[2].c_str(); *p; ++p )
        {
            byte color = (std::isupper(*p))        ? CASTLE_WHITE    : CASTLE_BLACK;
            byte side  = ('K' == std::toupper(*p)) ? CASTLE_KINGSIDE : CASTLE_QUEENSIDE;
            set_castle_right( color + side, true );
        }
    }

    // Field 4 - En Passant target square
    // - This is a square over which a pawn has just passed while moving two
    //   squares; 
    // - it is given in algebraic notation.
    // -  If there is no en passant target square, this field uses the character "-".
    // -  This is recorded regardless of whether there is a pawn in position to
    //    capture en passant.
    //
    set_en_passant( ( toks[3] == "-" ) ?Square::UNBOUNDED : Square(toks[3]) );

    // Field 5 - Halfmove Clock
    // - The number of halfmoves since the last capture or pawn advance, used for 
    //   the fifty-move rule.
    //
    set_half_move_clock( std::stoi(toks[4]) );

    // Field 6 - Fullmove Count
    // - The number of the full moves. It starts at 1 and is incremented after
    //   Black's move.
    //
    set_full_move_count( std::stoi(toks[5]) );
}

std::string Board::diagram() {
    std::stringstream ss;
    for ( short r = R8; r >= R1; --r ) {
        ss << char('a' + r) << ": ";
        for( short f = Fa; f <= Fh; ++f ) {
            PiecePtr pt = at(Square(r,f));
            ss << pt->glyph() << ' ';
        }
        ss << std::endl;
    }
    ss << "   1 2 3 4 5 6 7 8" << std::endl;
    return ss.str();
}

void Board::get_moves(MoveList& moves) {
    moves.clear();
    for ( auto p : _pm ) {
        p.second->get_moves( moves );
    }
}

const char *Board::initial_position_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";