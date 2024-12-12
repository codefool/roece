#pragma once
#include <cstdint>
#include <ostream>

union GameInfoPacked {
    uint32_t i;
    struct {
        uint32_t unused                      : 1;
        // en passant
        // If set, the pawn that rests on file en_passant_file moved two
        // positions. This signals that a pawn subject to en passant capture
        // exists, and does not mean that there is an opposing pawn that can
        // affect it. The value here is 0-7 to indicate file a-h, so > 7
        // indicates there is no en passant.
        uint32_t en_passant_ord               : 7; // square ord where pawn rests
        // Castling is possible only if the participating pieces have not
        // moved (among other rules, but have nothing to do with prior movement.)
        uint32_t castle_right_white_kingside  : 1;
        uint32_t castle_right_white_queenside : 1;
        uint32_t castle_right_black_kingside  : 1;
        uint32_t castle_right_black_queenside : 1;
        //
        // number of active pieces on the board (2..32)
        uint32_t piece_cnt                    : 6; // 2-32
        uint32_t on_move                      : 1; // 0=white on-move, 1=black
        uint32_t full_move_cnt                : 6; // 0-53
        uint32_t half_move_clock              : 7; // 0-127
    } f;

    GameInfoPacked();
    GameInfoPacked(uint32_t v);
    GameInfoPacked(const GameInfoPacked& o);
    bool operator==(const GameInfoPacked& o) const;
    bool operator!=(const GameInfoPacked& o) const;
    bool operator<(const GameInfoPacked& o) const;
};

struct PositionPacked
{
    GameInfoPacked gi;
    uint64_t       pop;   // population bitmap
    uint64_t       lo;    // lo 64-bits population info
    uint64_t       hi;    // hi 64-bits population info

    PositionPacked();
    PositionPacked(const PositionPacked& o);
    PositionPacked(uint32_t g, uint64_t p, uint64_t h, uint64_t l);
    bool operator==(const PositionPacked& o) const;
    bool operator!=(const PositionPacked& o) const;
    bool operator<(const PositionPacked& o) const;
    friend std::ostream& operator<<(std::ostream& os, const PositionPacked& pp);
};