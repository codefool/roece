#pragma once

#include <iostream>
#include <cstring>
#include <map>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "buildinfo.h"

extern const char *EMPTY_BOARD;
extern const char *INITIAL_POSITION;

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

#define SIDE_MASK  0x08
#define BLACK_MASK 0x08
#define PIECE_MASK 0x07

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
	MV_NONE             = 0x00,     // 0000 no legal move exists
	MV_MOVE             = 0x01,     // 0001 a legal move
    MV_MOVE_PAWN        = 0x02,     // 0010 a pawn legal move
	MV_CAPTURE          = 0x03,     // 0011 move results in capture
	MV_CASTLE_KINGSIDE  = 0x04,     // 0100 move is castle kingside
	MV_CASTLE_QUEENSIDE = 0x05,     // 0101 move is castle queenside
	MV_EN_PASSANT       = 0x06,     // 0110 move is pawn capture en passant 
	MV_PROM_QUEEN       = 0x07,     // 0111 move is pawn promotion to queen
	MV_PROM_BISHOP      = 0x08,     // 1000 move ""     ""    ""      bishop
	MV_PROM_KNIGHT      = 0x09,     // 1001 move ""     ""    ""      knight
	MV_PROM_ROOK        = 0x0a,     // 1010 move ""     ""    ""      rook
	MV_CAP_PROM_QUEEN   = 0x0b,     // 1011 promotion after capture to queen
	MV_CAP_PROM_BISHOP  = 0x0c,     // 1100 promotion ""    ""      "" bishop
	MV_CAP_PROM_KNIGHT  = 0x0d,     // 1101 promotion ""    ""      "" knight
	MV_CAP_PROM_ROOK    = 0x0e,     // 1110 promotion ""    ""      "" rook
    //
    MV_FAIL_NOT_ON_MOVE,            // moving piece is not on move
    MV_FAIL_BLOCKED                 // move is blocked by friendly piece
};

// Castle Rights 
// Does a side have the right (not necessarily the ability) to castle 
// on the king or queen side. A right to one side is lost if the rook
// on that side moves (8A3b). Both rights are lost for a side if its 
// king moves (8A3a).
enum CastleRight:uint8_t {
    CASTLE_RIGHT_WHITE_KINGSIDE  = 0x01,
    CASTLE_RIGHT_WHITE_QUEENSIDE = 0x02,
    CASTLE_RIGHT_BLACK_KINGSIDE  = 0x04,
    CASTLE_RIGHT_BLACK_QUEENSIDE = 0x08
};

enum Color       : byte { WHITE = 0,    BLACK, NONE };
enum CastleSide  : byte { KINGSIDE = 0, QUEENSIDE   };

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

struct Move {
    MoveAction action;
    Square     org;
    Square     dst;

    Move(MoveAction ma, Square o, Square d)
    : action(ma), org(o), dst(d)
    {}
    bool resultsInRootPosition(MoveAction ma);
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

#include "board.h"
#include "piece.h"

