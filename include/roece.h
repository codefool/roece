#pragma once

#include <iostream>
#include <cstring>
#include <map>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "buildinfo.h"

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
	MV_NONE             = 0x00,
	MV_MOVE             = 0x01,
	MV_CAPTURE          = 0x02,
	MV_CASTLE_KINGSIDE  = 0x03,
	MV_CASTLE_QUEENSIDE = 0x04,
	MV_EN_PASSANT       = 0x05,
	MV_PROM_QUEEN       = 0x06,
	MV_PROM_BISHOP      = 0x07,
	MV_PROM_KNIGHT      = 0x08,
	MV_PROM_ROOK        = 0x09,
	MV_CAP_PROM_QUEEN   = 0x0a,
	MV_CAP_PROM_BISHOP  = 0x0b,
	MV_CAP_PROM_KNIGHT  = 0x0c,
	MV_CAP_PROM_ROOK    = 0x0d,
    //
    MV_FAIL_NOT_ON_MOVE,        // moving piece is not on move
    MV_FAIL_BLOCKED             // move is blocked by friendly piece
};

// These constants combine to form indexes into Board::_castle_rights
// WHITE + KINGSIDE  == 0
// WHITE + QUEENSIDE == 1
// BLACK + KINGSIDE  == 2
// BLACK + QUEENSIDE == 3
enum CastleColor : byte { CASTLE_WHITE=0, CASTLE_BLACK=2 };
enum CastleSide  : byte { CASTLE_KINGSIDE=0, CASTLE_QUEENSIDE=1 };

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

struct MovePosition {
    Move move;
    // Position pos;
};

typedef std::vector<MovePosition> MovePositionList;

struct EvaluationResult {
    //Position origin;
    MovePositionList mp;
};

