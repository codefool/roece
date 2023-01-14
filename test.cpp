#include "roece.h"

int main() {
    Board b(false);
    b.from_fen("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1");
    std::cout << b.diagram() << std::endl;
    PiecePtr p = b.at(Square(R5,Fd));
    PieceList a = p->get_attackers(p);
    MoveList ml;
    b.get_moves(ml);

    short cnt(1);
    for( Move m : ml ) {
        std::cout << cnt << ' ' << m << std::endl;
        ++cnt;
    }
    Move x(MV_CASTLE_KINGSIDE, Square("e1"), Square("h1"));

    b.apply_move(x);
    std::cout << b.diagram() << std::endl;
   

    return 0;
}