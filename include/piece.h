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
    Piece();
    Piece( PieceType pt, Board *b, Color s = NONE );

public:
          Board&    board()  const;
    const PieceType type()   const;
    const Color     color()  const;
    const Square    square() const;
    const char      glyph()  const;
    const byte      range()  const;
    const uint8_t   toByte() const;

    const bool is_empty()  const;
    const bool is_knight() const;
    const bool is_white()  const;
    const bool is_black()  const;

    const bool is_enemy( PiecePtr ptr ) const;
    const bool is_friend( PiecePtr ptr ) const;

    PiecePtr ptr();

    void set_type(PieceType pt);
    void set_square(Square squ);
    void set_glyph();
    void set_board(Board* brd);

    virtual const DirList& get_dirs() const;
    virtual MoveAction move( const Move move );
    virtual void get_moves( MoveList& moves) const = 0;
    virtual bool can_attack( Square dst ) const = 0;
    
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

class King : public std::enable_shared_from_this<King>, public Piece {
public:
    King();
    King(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
    virtual MoveAction move( const Move move );
};

class Queen : public std::enable_shared_from_this<Queen>, public Piece {
public:
    Queen();
    Queen(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
};

class Bishop : public Piece {
public:
    Bishop();
    Bishop(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
};

class Knight : public Piece {
public:
    Knight();
    Knight(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
};

class Rook : public Piece {
public:
    Rook();
    Rook(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
};

class Pawn : public Piece {
public:
    Pawn();
    Pawn(Color c, Board* b);

public:
    virtual const DirList& get_dirs() const;
    virtual void get_moves( MoveList& moves ) const;
    virtual bool can_attack( Square dst ) const;
    MoveAction move(const Move move);
};

class Empty : public Piece {
public:
    Empty();
    virtual bool can_attack( Square dst ) const;
    virtual void get_moves( MoveList& moves ) const;
};

