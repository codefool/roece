#include "roece.h"
#include "iomanip"
#include "util.h"

std::vector<std::string> split(std::string target, std::string delimiter);

Board::Board(const char *fen) {
    if ( fen != EMPTY_BOARD ) {
        if ( strlen(fen) == 0 )
            fen = INITIAL_POSITION;
        from_fen(fen);
    }
}

Board::Board(const PositionPacked& pp) {
    unpack(pp,*this);
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
    return (_castle_rights & 0x0f) == 0;
}

void Board::clear_castle_rights() {
    _castle_rights = 0;
}

bool Board::get_castle_right( byte bit ) const {
    return (_castle_rights & bit) != 0;
}

std::string Board::get_castle_rights_string() const {
    std::stringstream ss;
    if ( none_can_castle() ) {
        ss << '-';
    } else {
        if ( get_castle_right( WHITE, KINGSIDE  ) ) ss << 'K';
        if ( get_castle_right( WHITE, QUEENSIDE ) ) ss << 'Q';
        if ( get_castle_right( BLACK, KINGSIDE  ) ) ss << 'k';
        if ( get_castle_right( BLACK, QUEENSIDE ) ) ss << 'q';
    }
    return ss.str();
}


// convert color and side attributes to an ordinal bit position
//        white  black
// king    0001   0010   2^0 2^1 s^2 2^3
// queen   0100   1000
// 
// color : white=0, black=1
// side  : king=0, queen=1
//
// ((color < 1) + side) gives the series {0,1,2,3}
// and 1 << ((color < 1) + side) gives series (1,2,4,8)
//
byte Board::castle_bit(Color c, CastleSide s ) const {
    // return (c == CASTLE_WHITE) 
    //        ? (s == CASTLE_KINGSIDE)
    //          ? CASTLE_RIGHT_WHITE_KINGSIDE : CASTLE_RIGHT_WHITE_QUEENSIDE
    //        : (s == CASTLE_KINGSIDE)
    //          ? CASTLE_RIGHT_BLACK_KINGSIDE : CASTLE_RIGHT_BLACK_QUEENSIDE;
    return (1 << ((c << 1) + s)) & 0x0f;
}

bool Board::get_castle_right( Color c, CastleSide s ) const {
    return get_castle_right( castle_bit(c, s) );
}

void Board::set_castle_right( byte bit, bool state ) {
    if (state) {
        _castle_rights |= bit;
    } else {
        _castle_rights &= ~bit;
    }
}

void Board::set_castle_right( Color c, CastleSide s, bool state ) {
    set_castle_right( castle_bit(c, s), state );
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

SeekResult Board::seek( Color side, Square src, Dir dir, short range ) {
    SeekResult ret;
    Square here = src;
    while ( range-- ) {
        here += offs[ dir ];
        if ( !here.in_bounds() )
            break;
        ret.path.push_back(here);
        PiecePtr occ = at(here);
        if ( ! occ->is_empty() ) {
            ret.occupant = occ;
            ret.rc = (occ->color() == side) 
                   ? SEEKRC_FOUND_FRIENDLY
                   : SEEKRC_FOUND_ENEMY;
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
    // remove the piece from the board
    remove( ptr );
    // and put it back where it belongs
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
// FEN of the starting position:
// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
// 
// Starts with a8, and proceeds left-to-right to h1.
// - Ranks are separated with '/'
// - Lower-case letters denote black pieces, upper-case white pieces
// - A number [1-8] denotes a count of consecutive blank squares
// - 
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
            Color      color = (std::isupper(*p))        ? WHITE    : BLACK;
            CastleSide side  = ('K' == std::toupper(*p)) ? KINGSIDE : QUEENSIDE;
            set_castle_right( color, side, true );
        }
    }

    // Field 4 - En Passant target square
    // - This is a square over which a pawn has just passed while advancing two
    //   squares on it's first move
    // - it is given in algebraic notation (a1-h8)
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
    // - "w" means that White is on move; "b" means that Black is on move
    //
    ss << "bw"[get_on_move() == WHITE]
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
    ss << get_castle_rights_string();
    // if ( none_can_castle() ) {
    //     ss << '-';
    // } else {
    //     if ( get_castle_right( WHITE, KINGSIDE  ) ) ss << 'K';
    //     if ( get_castle_right( WHITE, QUEENSIDE ) ) ss << 'Q';
    //     if ( get_castle_right( BLACK, KINGSIDE  ) ) ss << 'k';
    //     if ( get_castle_right( BLACK, QUEENSIDE ) ) ss << 'q';
    // }
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
    ss << std::endl << "   a b c d e f g h" << std::endl;
    for ( short r = R8; r >= R1; --r ) {
        ss << rank_glyph(r) << ": ";
        for( short f = Fa; f <= Fh; ++f ) {
            PiecePtr pt = at(Square(r,f));
            ss << pt->glyph() << ' ';
        }
        switch( r ) {
            case R8: ss << " ep:" << _en_passant;                break;
            case R7: ss << " cr:" << get_castle_rights_string(); break;
            case R6: ss << " om:" << ((_on_move)?"b":"w");       break;
            case R5: ss << " hm:" << _half_move_clock;           break;
            case R4: ss << " fm:" << _full_move_cnt;             break;
            case R3:
            case R2:
            case R1:
                break;
        };
        ss << std::endl;
    }
    return ss.str();
}

void Board::get_moves(MoveList& moves) {
    moves.clear();
    for ( auto p : _pm ) {
        if ( p.second->color() == get_on_move() )
            p.second->get_moves( moves );
    }
}

#define NO_EN_PASSANT 0x7f

PositionPacked Board::pack() const {
    PositionPacked ret;
    ret.gi.f.castle_right_white_kingside  = get_castle_right(WHITE, KINGSIDE );
    ret.gi.f.castle_right_white_queenside = get_castle_right(WHITE, QUEENSIDE);
    ret.gi.f.castle_right_black_kingside  = get_castle_right(BLACK, KINGSIDE );
    ret.gi.f.castle_right_black_queenside = get_castle_right(BLACK, QUEENSIDE);
    ret.gi.f.on_move         = _on_move;
    ret.gi.f.piece_cnt       = _pm.size();
    ret.gi.f.half_move_clock = get_half_move_clock();
    ret.gi.f.full_move_cnt   = get_full_move_count();
    ret.gi.f.en_passant_ord  = has_en_passant()
                             ? get_en_passant().ordinal()
                             : NO_EN_PASSANT;

    // the Board piece map is keyed by ordinal [0..63], and being
    // and ordered collection guaranteed to be in order from 0,0 (a1)
    // to 7,7 (h8.)
    // 
    // Pack the population as a 64-bit map depicting which squares
    // [0..63] contain pieces, hashed by ordinal number. Hence, a0 (0)
    // is the LSB, and h8 (63) is the MSB.
    //
    //        h       g       f       e       d       c       b       a
    // 8765432187654321876543218765432187654321876543218765432187654321
    // 6                                                              0 
    // 3 
    //
    // There can be max 32 pieces on the board - whose type information
    // can be packed into four bit (nibble.)
    uint8_t  bitcnt{0};    
    uint64_t pop{0};
    for ( auto piece : _pm ) {
        uint8_t ord = piece.first.ordinal(); // 0..63
        pop |= (uint64_t)(1ull << ord);
        uint8_t pt  = piece.second->toByte();
        int idx{bitcnt/2};                           // 0,0,1,1,2,2,...  
        if ( bitcnt & 1 )                            // if odd set right, else left
            ret.pieces.b[idx].r = pt;
        else
            ret.pieces.b[idx].l = pt;
        bitcnt++;
    }
    ret.pop = pop;
    return ret;
}

void Board::init(const PositionPacked& pp) {
    unpack( pp, *this );
}

void Board::unpack(const PositionPacked& pp, Board& ret) {
    ret._pm.clear();
    ret.set_castle_right(WHITE, KINGSIDE,  pp.gi.f.castle_right_white_kingside );
    ret.set_castle_right(WHITE, QUEENSIDE, pp.gi.f.castle_right_white_queenside);
    ret.set_castle_right(BLACK, KINGSIDE,  pp.gi.f.castle_right_black_kingside );
    ret.set_castle_right(BLACK, QUEENSIDE, pp.gi.f.castle_right_black_queenside);
    ret.set_on_move(pp.gi.f.on_move?BLACK:WHITE);

    if ( pp.gi.f.en_passant_ord != NO_EN_PASSANT )
        ret.set_en_passant(Square(pp.gi.f.en_passant_ord));
    else
        ret.clear_en_passant();

    ret.set_half_move_clock(pp.gi.f.half_move_clock);
    ret.set_full_move_count(pp.gi.f.full_move_cnt);

    uint64_t pop{pp.pop};
    uint8_t  bitcnt{0};
    uint64_t bit{1};
    for ( uint8_t ord{0}; bit; ++ord, bit <<= 1ull ) {
        if ( pop & bit ) {
            int idx{bitcnt/2};                  // 0,0,1,1,2,2,...
            uint8_t pt  = ( bitcnt & 1 )        // odd
                        ? pp.pieces.b[idx].r
                        : pp.pieces.b[idx].l;

            auto pptr = Piece::fromByte(pt);
            _pm[ord] = pptr;
            pptr->set_board(&ret);
            pptr->set_square(Square(ord));
            bitcnt++;
        }
    }
}


EvaluationResult Board::evaluate() {
    EvaluationResult result;

    return result;
}

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
