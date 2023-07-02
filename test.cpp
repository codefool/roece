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

    short cnt(1);
    for( Move m : ml ) {
        std::cout << cnt << ' ' << m << std::endl;
        ++cnt;
    }
    Move x(MV_EN_PASSANT, Square("f4"), Square("e3"));
    
    PositionPacked pp = b.pack();

    Board c = b.unpack(pp);

    b.apply_move(x);
    std::cout << b.diagram() << std::endl << b.fen() << std::endl
              << pp << std::endl;

    std::cout << c.diagram() << std::endl << c.fen() << std::endl
              << c.pack() << std::endl;
   
    return 0;
}