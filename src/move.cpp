#include "roece.h"

MovePacked::MovePacked()
: w{0}
{}

MovePacked::MovePacked(const Move& m)
: w{0}
{
    b.ma  = (m.action & 0x0f);
    b.org = m.org.ordinal();
    b.dst = m.dst.ordinal();
}

// These move actions produce a new root position. In general,
// these are actions that are a pawn move and/or piece capture.
bool Move::resultsInRootPosition(MoveAction ma) {
    switch(ma) {
        // pawn moves
        case MV_MOVE_PAWN:
        case MV_EN_PASSANT:
        // pawn promotions
        case MV_PROM_QUEEN:
        case MV_PROM_BISHOP:
        case MV_PROM_KNIGHT:
        case MV_PROM_ROOK:
        // piece captures
        case MV_CAPTURE:
        // piece capture with promotion
        case MV_CAP_PROM_QUEEN:
        case MV_CAP_PROM_BISHOP:
        case MV_CAP_PROM_KNIGHT:
        case MV_CAP_PROM_ROOK:
            return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Move& mv) {
    switch (mv.action) {
        case MV_NONE            : os << "MV_NONE";             break;
        case MV_MOVE            : os << "MV_MOVE";             break;
        case MV_MOVE_PAWN       : os << "MV_MOVE_PAWN";        break;
        case MV_CAPTURE         : os << "MV_CAPTURE";          break;
        case MV_CASTLE_KINGSIDE : os << "MV_CASTLE_KINGSIDE";  break;
        case MV_CASTLE_QUEENSIDE: os << "MV_CASTLE_QUEENSIDE"; break;
        case MV_EN_PASSANT      : os << "MV_EN_PASSANT";       break;
        case MV_PROM_QUEEN      : os << "MV_PROM_QUEEN";       break;
        case MV_PROM_BISHOP     : os << "MV_PROM_BISHOP";      break;
        case MV_PROM_KNIGHT     : os << "MV_PROM_KNIGHT";      break;
        case MV_PROM_ROOK       : os << "MV_PROM_ROOK";        break;
        case MV_CAP_PROM_QUEEN  : os << "MV_CAP_PROM_QUEEN";   break;
        case MV_CAP_PROM_BISHOP : os << "MV_CAP_PROM_BISHOP";  break;
        case MV_CAP_PROM_KNIGHT : os << "MV_CAP_PROM_KNIGHT";  break;
        case MV_CAP_PROM_ROOK   : os << "MV_CAP_PROM_ROOK";    break;
    }

    // os << ' ';

    // switch (mv.result) {
    //     case MR_NONE        : os << "MR_NONE"; break;
    //     case MR_CHECK       : os << "MR_CHECK"; break;  
    //     case MR_DOUBLE_CHECK: os << "MR_DOUBLE_CHECK"; break;  
    // }

    os << ' ' << mv.org << "-->" << mv.dst;

    return os;
}
