#pragma once
#include <cstdint>
#include <ostream>
#include <cstring>

#pragma pack(1)

struct Mask {
    uint32_t base_mask;
    uint32_t and_mask;
    uint32_t shift;
};

struct GameInfoPacked {
    // gcc doesn't appear to handle bit fields correctly, so have to do it manually.
    //         2         1         0         0
    // .... ...4 .... ...6 .... ...8 .... ...0
    // x... .... .... .... .... .... .... .... unused
    // .xxx xxxx .... .... .... .... .... .... en passant ord (0..63)
    // .... .... x... .... .... .... .... .... castle right white kingside
    // .... .... .x.. .... .... .... .... .... castle right white queenside
    // .... .... ..x. .... .... .... .... .... castle right black kingside
    // .... .... ...x .... .... .... .... .... castle right black queenside
    // .... .... .... xxxx xx.. .... .... .... piece count (2..32)
    // .... .... .... .... ..x. .... .... .... on-move (0=white, 1=black)
    // .... .... .... .... ...x xxxx x... .... full move count (0..53)
    // .... .... .... .... .... .... .xxx xxxx half-move count (0..127)
    //
    enum FieldName {
        EN_PASSANT_ORD = 0,
        CASTLE_RIGHT_WHITE_KINGSIDE,
        CASTLE_RIGHT_WHITE_QUEENSIDE,
        CASTLE_RIGHT_BLACK_KINGSIDE,
        CASTLE_RIGHT_BLACK_QUEENSIDE,
        ALL_CASTLE_RIGHTS,
        PIECE_CNT,
        ON_MOVE,
        FULL_MOVE_CNT,
        HALF_MOVE_CLOCK,
        FIELD_CNT
    };

    static const Mask _masks[FIELD_CNT];

    // pairs of bias from zero, and width
    uint32_t i;

    inline uint32_t clear(FieldName fld) {
        return i & ~_masks[fld].and_mask;
    }

    inline uint32_t set(FieldName f, uint32_t val) {
        auto work = clear(f);
        work |= ((val & _masks[f].base_mask) << _masks[f].shift);
        i = work;
        return i;
    }

    inline uint32_t get(FieldName f) const {
        return ((i >> _masks[f].shift) & _masks[f].base_mask);
    }

    inline uint32_t getEnPassantOrd() const { return get(EN_PASSANT_ORD); }
    inline uint32_t getEnPassantOrd() const { return get(EN_PASSANT_ORD); }



    inline void     setEnPassantOrd(uint32_t o) { set(EN_PASSANT_ORD, o); }

}

// union GameInfoPacked {
//     static uint32_t ExcludeClockMask;
//     uint32_t i;
//     struct {
//         uint32_t unused                       : 1;
//         // en passant
//         // If set, the pawn that rests on file en_passant_file moved two
//         // positions. This signals that a pawn subject to en passant capture
//         // exists, and does not mean that there is an opposing pawn that can
//         // affect it. The value here is 0-7 to indicate file a-h, so > 7
//         // indicates there is no en passant.
//         uint32_t en_passant_ord               : 7; // square ord where pawn rests
//         // Castling is possible only if the participating pieces have not
//         // moved (among other rules, but have nothing to do with prior movement.)
//         uint32_t castle_right_white_kingside  : 1;
//         uint32_t castle_right_white_queenside : 1;
//         uint32_t castle_right_black_kingside  : 1;
//         uint32_t castle_right_black_queenside : 1;
//         //
//         // number of active pieces on the board (2..32)
//         uint32_t piece_cnt                    : 6; // 2-32
//         uint32_t on_move                      : 1; // 0=white on-move, 1=black
//         uint32_t full_move_cnt                : 6; // 0-53
//         uint32_t half_move_clock              : 7; // 0-127
//     } f;

//     GameInfoPacked();
//     GameInfoPacked(uint32_t v);
//     GameInfoPacked(const GameInfoPacked& o);
//     bool operator==(const GameInfoPacked& o) const;
//     bool operator!=(const GameInfoPacked& o) const;
//     bool operator<(const GameInfoPacked& o) const;
// };

// PositionPacked
//
// 
struct PositionPacked
{
    GameInfoPacked gi;    // 32-bits
    uint64_t       pop;   // population bitmap
    union {
        uint64_t w[2];
        // traversing the board from top-bottom (r8-r1) and 
        // left-to-right (fa-fh), the piece type is packed
        // a b c d e f g h
        // l r l r l r l r
        // l r l r l r l r
        struct {
            uint8_t l:4;
            uint8_t r:4;
        } b[16];
    } pieces;

    PositionPacked();
    PositionPacked(const PositionPacked& o);
    PositionPacked(uint32_t g, uint64_t p, uint64_t h, uint64_t l);
    std::string hexString() const;
    bool operator==(const PositionPacked& rhs) const;
    bool operator!=(const PositionPacked& rhs) const;
    bool operator<(const PositionPacked& rhs) const;
    bool compareNoClocks(const PositionPacked& rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const PositionPacked& pp);
};

struct CrossReference {
   
    PositionPacked p;
};

#pragma pack()
