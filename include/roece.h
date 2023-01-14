#pragma once

#include <iostream>
#include <cstring>
#include <map>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

typedef signed char byte;

enum PieceType : uint8_t {
    PT_EMPTY    = 0x00,
    PT_KING     = 0x01,
    PT_QUEEN    = 0x02,
    PT_BISHOP   = 0x03,
    PT_KNIGHT   = 0x04,
    PT_ROOK     = 0x05,
    PT_PAWN     = 0x06,
    PT_PAWN_OFF = 0x07,
    PT_NONE     = 0x80
};

enum Color { WHITE = 0, BLACK, NONE };

enum Dir { 
    UP,   DN,   LFT,  RGT,  UPR,  UPL,  DNR,  DNL,
    KLUP, KUPL, KUPR, KRUP, KRDN, KDNR, KDNL, KLDN,
    NOWHERE
};

enum SeekResultCode {
    SEEKRC_NONE,
    SEEKRC_FOUND_ENEMY,
    SEEKRC_FOUND_FRIENDLY
};

enum MoveAction : uint8_t {
	MV_NONE             = 0,
	MV_MOVE             = 1,
	MV_CAPTURE          = 2,
	MV_CASTLE_KINGSIDE  = 3,
	MV_CASTLE_QUEENSIDE = 4,
	MV_EN_PASSANT       = 5,
	MV_PROM_QUEEN       = 6,
	MV_PROM_BISHOP      = 7,
	MV_PROM_KNIGHT      = 8,
	MV_PROM_ROOK        = 9,
    //
    MV_FAIL_NOT_ON_MOVE,        // moving piece is not on move
    MV_FAIL_BLOCKED             // move is blocked by friendly piece
};

// These constants combine to form indexes into Board::_castle_rights
// WHITE + KINGSIDE  == 0
// WHITE + QUEENSIDE == 1
// BLACK + KINGSIDE  == 2
// BLACK + QUEENSIDE == 3
const byte CASTLE_WHITE(0);
const byte CASTLE_BLACK(2);
const byte CASTLE_KINGSIDE(0);
const byte CASTLE_QUEENSIDE(1);

const byte CASTLE_WHITE_KINGSIDE ( CASTLE_WHITE + CASTLE_KINGSIDE  );
const byte CASTLE_WHITE_QUEENSIDE( CASTLE_WHITE + CASTLE_QUEENSIDE );
const byte CASTLE_BLACK_KINGSIDE ( CASTLE_BLACK + CASTLE_KINGSIDE  );
const byte CASTLE_BLACK_QUEENSIDE( CASTLE_BLACK + CASTLE_QUEENSIDE );

enum Rank : byte { R1=0, R2, R3, R4, R5, R6, R7, R8 };
enum File : byte { Fa=0, Fb, Fc, Fd, Fe, Ff, Fg, Fh };

#define rank_glyph(r) ((char)('1'+r))
#define file_glyph(f) ((char)('a'+f))

class Piece;
class Board;
class Move;
class Square;

typedef std::shared_ptr<Piece>    PiecePtr;
typedef std::vector<PiecePtr>     PieceList;
typedef std::vector<Square>       SquareList;
typedef std::map<Square,PiecePtr> PiecePtrMap;
typedef std::pair<byte, byte>     Offset;
typedef std::vector<Dir>          DirList;
typedef std::vector<Move>         MoveList;

extern const Offset offs[];

extern const DirList none_dirs;
extern const DirList axes_dirs;    
extern const DirList diag_dirs;    
extern const DirList omni_dirs;    
extern const DirList knight_dirs;  
extern const DirList pawn_black; 
extern const DirList pawn_white;  

struct SeekResult {
    SeekResultCode rc;
    SquareList     path;
    PiecePtr       occupant;
    SeekResult()
    : rc(SEEKRC_NONE), occupant(nullptr)
    {}
};

#include "square.h"
#include "board.h"
#include "piece.h"

struct Move {
    MoveAction action;
    Square     org;
    Square     dst;

    Move(MoveAction ma, Square o, Square d)
    : action(ma), org(o), dst(d)
    {}
    friend std::ostream& operator<<(std::ostream& os, const Move& move);
};


