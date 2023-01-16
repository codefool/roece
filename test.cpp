#include "roece.h"

int main() {
    Board b(false);
    b.from_fen("1nrN4/2P5/8/8/4Pp2/8/2p5/1NRn4 w - e4 0 1");
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
    Move x(MV_EN_PASSANT, Square("f4"), Square("e3"));

    b.apply_move(x);
    std::cout << b.diagram() << std::endl << b.fen() << std::endl;
   
    return 0;
}