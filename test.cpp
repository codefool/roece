#include "roece.h"

int main() {
    std::cout << bi << std::endl;
    // Board b(false);
    // b.from_fen("1nrN4/2P5/8/8/4Pp2/8/2p5/1NRn4 w - e4 0 1");
    Board b("");
    std::cout << b.diagram() << std::endl;
    PiecePtr p = b.at(Square(R5,Fd));
    PieceList a = p->get_attackers(p);
    MoveList ml;
    b.get_moves(ml);
    for ( auto mv : ml ) {
        Board c(b);
        c.apply_move(mv);
        std::cout << c.fen() << std::endl << c.diagram() << std::endl << mv << std::endl;
    }

    std::cout << b.diagram() << std::endl << b.fen() << std::endl;

    return 0;
}