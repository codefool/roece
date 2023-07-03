#include "roece.h"
#include "util.h"

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

Board::Board() {}

Board::Board(std::string fen) {
    if ( fen.length() == 0 )
        fen = initial_position_fen;
    from_fen(fen);
}

Board::Board(PositionPacked& pp) {
    unpack(pp);
}

Board::Board(const Board& other) {
    set_on_move( other.get_on_move() );
    set_en_passant( other.get_en_passant() );
    set_half_move_clock( other.get_half_move_clock() );
    set_full_move_count( other.get_full_move_count() );
    for ( short idx(0); idx < 4; ++idx ) 
        set_castle_right( idx, other.get_castle_right(idx) );
    for ( auto entry : other._pm ) {
        const Square squ( entry.first );
        PiecePtr     o_ptr( entry.second );
        PiecePtr     ptr( Piece::factory( o_ptr->type(), this, o_ptr->color() ));
        ptr->set_square(squ);
        set( squ, ptr );
    }
}

MoveAction Board::apply_move(const Move& move) {
    PiecePtr src = at(move.org);
    Square   eps = get_en_passant();
    if ( src->is_empty() )
        return MV_NONE;
    MoveAction ret = src->move(move);
    inc_half_move_clock();
    if (src->is_black())
        inc_full_move_count();
    // clear en passant if en passant was not set this move
    if ( get_en_passant() == eps )
        clear_en_passant();
    toggle_on_move();
    return ret;
}

Color Board::get_on_move() const {
    return _on_move;
}

void Board::set_on_move(Color c) {
    _on_move = c;
}

void Board::toggle_on_move() {
    set_on_move((_on_move == WHITE)?BLACK:WHITE);
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

bool Board::get_castle_right( CastleColor c, CastleSide s ) const {
    return get_castle_right( c + s );
}

void Board::set_castle_right( byte idx, bool state ) {
    _castle_rights[ idx ] = state;
}

void Board::set_castle_right( CastleColor c, CastleSide s, bool state ) {
    set_castle_right( c + s, state );
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
    _en_passant = squ.file();
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

PiecePtr Board::at(Square squ) const {
    auto itr = _pm.find(squ);
    if ( itr == _pm.end() )
        return Piece::EMPTY;
    return itr->second;
}

PiecePtr Board::at(byte rank, byte file) const {
    return at(Square(rank,file));
}

PiecePtr Board::make_piece( PieceType pt, Color c ) {
    return Piece::factory(pt, this, c);
}

void Board::set( Square squ, PiecePtr ptr ) {
    // if the target square isn't empty - empty it
    remove( at(squ) );
    // erase the src object from the board
    remove( ptr );
    // and put it back
    _pm[squ] = ptr;
    // and record new location in the piece
    ptr->set_square(squ);
}

void Board::remove(PiecePtr ptr) {
    if ( !ptr->is_empty() && !ptr->square().is_unbounded() )
        _pm.erase(ptr->square());
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
            Color     color = (std::islower(*p)?BLACK:WHITE);
            switch(std::toupper(*p)) {
                case 'K': pt = PT_KING;   break;
                case 'Q': pt = PT_QUEEN;  break;
                case 'B': pt = PT_BISHOP; break;
                case 'N': pt = PT_KNIGHT; break;
                case 'R': pt = PT_ROOK;   break;
                case 'P': pt = PT_PAWN;   break;
            }
            set( Square(rank, file), Piece::factory( pt, this, color) );
            ++file;
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
    set_on_move(( std::tolower(toks[1].at(0)) == 'b' ) ? BLACK : WHITE);

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
    // - This is a square over which a pawn has just passed while advancing two
    //   squares on it's first move
    // - it is given in algebraic notation.
    // - If there is no en passant target square, this field uses the character "-".
    // - This is recorded regardless of whether there is a pawn in position to
    //   exercise en passant.
    //
    if ( toks[3] == "-" )
        clear_en_passant();
    else
        set_en_passant(Square(toks[3]));

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

std::string Board::fen() const {
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
    std::string fen;
    std::stringstream ss;
    for ( short rank = R8; rank >= R1; --rank ) {
        short cnt(0);
        for ( short file = Fa; file <= Fh; ++file ) {
            PiecePtr ptr = at(rank,file);
            if ( ptr->is_empty() ) {
                cnt++;
            } else {
                if ( cnt ) {
                    ss << cnt;
                    cnt = 0;
                }
                ss << ptr->glyph();
            }
        }
        if ( cnt )
            ss << cnt;
        if ( rank > R1 )
            ss << '/';
    }
    ss << ' ';

    // Field 2 - Active Color
    // - "w" means that White is to move; "b" means that Black is to move
    //
    ss << ((get_on_move() == BLACK) ? 'b' : 'w')
       << ' ';

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
    if ( none_can_castle() ) {
        ss << '-';
    } else {
        if ( get_castle_right( CASTLE_WHITE, CASTLE_KINGSIDE )  ) ss << 'K';
        if ( get_castle_right( CASTLE_WHITE, CASTLE_QUEENSIDE ) ) ss << 'Q';
        if ( get_castle_right( CASTLE_BLACK, CASTLE_KINGSIDE )  ) ss << 'k';
        if ( get_castle_right( CASTLE_BLACK, CASTLE_QUEENSIDE ) ) ss << 'q';
    }
    ss << ' ' ;

    // Field 4 - En Passant target square
    // - This is a square over which a pawn has just passed while moving two
    //   squares; 
    // - it is given in algebraic notation.
    // -  If there is no en passant target square, this field uses the character "-".
    // -  This is recorded regardless of whether there is a pawn in position to
    //    capture en passant.
    //
    if ( has_en_passant() ) {
        ss << get_en_passant();
    } else {
        ss << '-';
    }
    ss << ' ';

    // Field 5 - Halfmove Clock
    // - The number of halfmoves since the last capture or pawn advance, used for 
    //   the fifty-move rule.
    //
    ss << get_half_move_clock() << ' ';

    // Field 6 - Fullmove Count
    // - The number of the full moves. It starts at 1 and is incremented after
    //   Black's move.
    //
    ss << get_full_move_count();

    return ss.str();
}

std::string Board::diagram() {
    std::stringstream ss;
    ss << "   a b c d e f g h" << std::endl;
    for ( short r = R8; r >= R1; --r ) {
        ss << rank_glyph(r) << ": ";
        for( short f = Fa; f <= Fh; ++f ) {
            PiecePtr pt = at(Square(r,f));
            ss << pt->glyph() << ' ';
        }
        ss << std::endl;
    }
    return ss.str();
}

void Board::get_moves(MoveList& moves) {
    moves.clear();
    for ( auto p : _pm ) {
        p.second->get_moves( moves );
    }
}

const char *Board::initial_position_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

#define NO_EN_PASSANT 0x7f

PositionPacked Board::pack() const {
    PositionPacked ret;
    ret.gi.f.castle_right_white_kingside  = (get_castle_right(CASTLE_WHITE, CASTLE_KINGSIDE))?1:0;
    ret.gi.f.castle_right_white_queenside = (get_castle_right(CASTLE_WHITE, CASTLE_QUEENSIDE))?1:0;
    ret.gi.f.castle_right_black_kingside  = (get_castle_right(CASTLE_BLACK, CASTLE_KINGSIDE))?1:0;
    ret.gi.f.castle_right_black_queenside = (get_castle_right(CASTLE_BLACK, CASTLE_QUEENSIDE))?1:0;
    ret.gi.f.on_move = _on_move;
    ret.gi.f.piece_cnt = _pm.size();
    ret.gi.f.half_move_clock = get_half_move_clock();
    ret.gi.f.full_move_cnt = get_full_move_count();
    ret.gi.f.en_passant_ord = has_en_passant()
                            ? get_en_passant().ordinal()
                            : NO_EN_PASSANT;

    uint64_t   pop{0};
    uint32_t   bitcnt{0};
    uint8_t    map[32];
    uint64_t   buff[2];
    uint8_t    bit{0};
    std::map<uint8_t,uint8_t> mapp;

    for ( auto piece : _pm ) {
        uint8_t ord = 63 - piece.first.ordinal();
        pop |= (uint64_t)(1ULL << ord);
        mapp[ord] = piece.second->toByte();
    }

    // 
    for ( auto itr = mapp.rbegin(); itr != mapp.rend(); ++itr ) {
        map[bitcnt++] = itr->second;
    }

    ret.pop = pop;
    pack_array(map, (uint8_t*)buff, bitcnt);
    ret.hi = buff[0];
    ret.lo = buff[1];

    return ret;
}

Board Board::unpack(PositionPacked& pp) {
    Board ret;

    uint64_t pop{pp.pop};
    uint8_t  map[32];
    uint64_t buff[2] = {pp.hi, pp.lo};
    std::memset(map, 0x00, sizeof(map));
    unpack_array((uint8_t*)buff, map, 32);
    uint8_t *pmap = map;
    for(uint8_t ord{0}; pop; ++ord, pop >>= 1) {
        if ( pop & 1 ) {
            uint8_t   pb = *pmap++;
            PieceType pt = static_cast<PieceType>( pb & PIECE_MASK);
            Color     c  = static_cast<Color>    ((pb & SIDE_MASK) != 0);
            ret._pm[Square(ord)] = Piece::factory(pt, &ret, c);
        }
    }

    ret.set_castle_right(CASTLE_WHITE, CASTLE_KINGSIDE, pp.gi.f.castle_right_white_kingside);
    ret.set_castle_right(CASTLE_WHITE, CASTLE_QUEENSIDE, pp.gi.f.castle_right_white_queenside);
    ret.set_castle_right(CASTLE_BLACK, CASTLE_KINGSIDE, pp.gi.f.castle_right_black_kingside);
    ret.set_castle_right(CASTLE_BLACK, CASTLE_QUEENSIDE, pp.gi.f.castle_right_black_queenside);
    ret.set_on_move(pp.gi.f.on_move?BLACK:WHITE);
    if ( pp.gi.f.en_passant_ord != NO_EN_PASSANT )
        ret.set_en_passant(Square(pp.gi.f.en_passant_ord));
    else
        ret.clear_en_passant();
    ret.set_half_move_clock(pp.gi.f.half_move_clock);
    ret.set_full_move_count(pp.gi.f.full_move_cnt);

    return ret;
}


EvaluationResult Board::evaluate() {
    EvaluationResult result;

    return result;
}