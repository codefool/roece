#pragma once

#include <iostream>
#include <cassert>
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
    PT_NONE     = 0x00,
    PT_KING     = 0x01,
    PT_QUEEN    = 0x02,
    PT_BISHOP   = 0x03,
    PT_KNIGHT   = 0x04,
    PT_ROOK     = 0x05,
    PT_PAWN     = 0x06
};

extern const char *PieceName[];

#define SIDE_MASK  0x08
#define BLACK_MASK 0x08
#define PIECE_MASK 0x07

enum Dir {
    // compass moves
    UP,   DN,   LFT,  RGT,  UPR,  UPL,  DNR,  DNL,
    // knight moves
    NLUP, NUPL, NUPR, NRUP, NRDN, NDNR, NDNL, NLDN,
    // no move at all
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
    // failure conditions
    //
    MV_FAIL_NOT_ON_MOVE = 0x80,     // moving piece is not on move
    MV_FAIL_BLOCKED,                // move is blocked by friendly piece
    MV_FAIL_NOT_CHECK_RESPONSE,     // move is not a response to king in check (12C)
    MV_FAIL_CAUSES_CHECK,           // move would place king in check (12E)
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

enum Color       : byte { WHITE = 0,    BLACK, DRAB };
enum CastleSide  : byte { KINGSIDE = 0, QUEENSIDE   };

enum Rank : byte { R1=0, R2, R3, R4, R5, R6, R7, R8 };
enum File : byte { Fa=0, Fb, Fc, Fd, Fe, Ff, Fg, Fh };

#define rank_glyph(r) ((char)('1'+r))
#define file_glyph(f) ((char)('a'+f))

class Piece;
class Board;
class Move;
class Square;

typedef std::shared_ptr<Piece>     PiecePtr;
typedef std::vector<PiecePtr>      PieceList;
typedef std::vector<Square>        SquareList;
typedef std::map<Square,PiecePtr>  PiecePtrMap;
typedef std::pair<byte, byte>      Offset;
typedef std::vector<Dir>           DirList;
typedef std::vector<Move>          MoveList;

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

struct BoardState {
    Color       _on_move;
    byte        _castle_rights;
    Square      _en_passant;
    short       _half_move_clock;
    short       _full_move_cnt;

    BoardState();
    BoardState(const BoardState& other);
    BoardState& operator=(const BoardState& other);
};

// The MoveResult reports the result of apply a move to a baord.
// The src/trg/bs records the state of the affected piece(s) 
// and the board state before the move to make it easier to back
// out.
struct MoveResult {
    MoveAction action;  // result of the move
    PiecePtr   src;     // piece being moved
    PiecePtr   trg;     // optional affected piece (capture)
    BoardState bs;      // board state 
    Square     squSrc;  // where source ended up
    Square     squTrg;  // where traget ended up
    MoveResult();
    MoveResult(PiecePtr src, PiecePtr trg, Board* b);
};

struct Move {
    MoveAction action;
    Square     org;
    Square     dst;

    Move(){}
    Move(MoveAction ma, Square o, Square d)
    : action(ma), org(o), dst(d)
    {}
    bool resultsInRootPosition(MoveAction ma);
    friend std::ostream& operator<<(std::ostream& os, const Move& move);
};

#pragma pack(1)
// MovePacked
//
// xxxx .... .... .... MoveAction
// .... xxxx xx.. .... Origin Square (0..63)
// .... .... ..xx xxxx Destination Square (0..63)
// 
union MovePacked {
    uint16_t w;
    struct {
        uint16_t ma:4;
        uint16_t org:6;
        uint16_t dst:6;
    } b;
    MovePacked();
    MovePacked(const Move& m);
};
#pragma pack()

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

