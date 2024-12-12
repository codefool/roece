#pragma once
#include "roece.h"
#include "position.h"

class Board {
private:
    std::map<Square, PiecePtr> _pm;
    Color  _on_move;
    byte   _castle_rights;
    Square _en_passant;
    short  _half_move_clock;
    short  _full_move_cnt;

public:
    Board(const char *fen = EMPTY_BOARD);
    Board(PositionPacked& pp);
    Board(const Board& other);
    void init(PositionPacked& pp);
    Color get_on_move() const;
    void set_on_move(Color s);
    void toggle_on_move();
    bool none_can_castle() const;
    void clear_castle_rights();
    byte castle_bit(Color c, CastleSide s ) const;
    bool get_castle_right( byte bit ) const;
    bool get_castle_right( Color c, CastleSide s) const;
    void set_castle_right( byte bit, bool state );
    void set_castle_right( Color c, CastleSide s, bool state);
    bool has_en_passant() const;
    Square get_en_passant() const;
    void clear_en_passant();
    void set_en_passant( Square squ );
    short get_half_move_clock() const;
    void set_half_move_clock( short val );
    void clear_half_move_clock();
    void inc_half_move_clock();
    short get_full_move_count() const;
    void set_full_move_count( short val );
    void clear_full_move_count();
    void inc_full_move_count();

    Board deep_copy() const;
    MoveAction apply_move(const Move& move );

    SeekResult seek( PiecePtr src, Dir dir, short range );
    PiecePtr at(Square squ) const;
    PiecePtr at(byte rank, byte file) const;
    PiecePtr make_piece( PieceType pt, Color s );
    void set( Square squ, PiecePtr ptr );
    void remove(PiecePtr ptr);
    void from_fen(const std::string& fen);
    std::string fen() const;
    std::string diagram();

    PositionPacked pack() const;
    void unpack(PositionPacked& pp, Board& ret);

    EvaluationResult evaluate();

    void get_moves(MoveList& moves);
};
