#pragma once

#include "roece.h"

class Piece {
private:
    PieceType _t;
    Color     _c;
    char      _g;
    Square    _s;
    Board*    _b;

protected:
    Piece( PieceType pt, Board *b, Color s = NONE );

public:
          Board&    board()  const;
    const PieceType type()   const;
    const Color     color()  const;
    const Square    square() const;
    const char      glyph()  const;
    const byte      range()  const;

    const bool is_empty()  const;
    const bool is_knight() const;
    const bool is_white()  const;
    const bool is_black()  const;

    const bool is_enemy( PiecePtr ptr ) const;
    const bool is_friend( PiecePtr ptr ) const;

    void set_square(Square squ);

    void set_glyph();

    void set_board(Board* brd);

    virtual const DirList& get_dirs() const;
    virtual bool move( Square dst );
    virtual bool promote( PieceType pt );
    virtual void get_moves( MoveList& moves) const = 0;
    virtual bool can_attack( Square dst ) const = 0;
    virtual void apply_move( const Move& move ) = 0;
    
    PieceList get_attackers(PiecePtr trg);

protected:
    bool can_diag_attack( Square dst ) const;
    bool can_axes_attack( Square dst ) const;
    bool can_omni_attack( Square dst ) const;
    void get_diag_moves( MoveList& moves ) const;
    void get_axes_moves( MoveList& moves ) const;
    void get_omni_moves( MoveList& moves ) const;
    void get_dirs_moves( const DirList& dirs, MoveList& moves ) const;

public:
    static PiecePtr factory(PieceType pt, Board* b, Color s = NONE );
    static PiecePtr EMPTY;
    static const char *glyphs;
    static const byte ranges[8];
};

class King : public Piece {
public:
    King(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
    virtual void apply_move( const Move& move );
};

class Queen : public Piece {
public:
    Queen(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
    virtual void apply_move( const Move& move );
};

class Bishop : public Piece {
public:
    Bishop(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
    virtual void apply_move( const Move& move );
};

class Knight : public Piece {
public:
    Knight(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
    virtual void apply_move( const Move& move );
};

class Rook : public Piece {
public:
    Rook(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
    virtual void apply_move( const Move& move );
};

class Pawn : public Piece {
public:
    Pawn(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
    virtual void apply_move( const Move& move );
};

class Empty : public Piece {
public:
    Empty();
    virtual bool can_attack( Square dst ) const;
    virtual void get_moves( MoveList& moves ) const;
    virtual void apply_move( const Move& move );
};

