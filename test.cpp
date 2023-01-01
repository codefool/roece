#include "roece.h"

int main() {
    Board b;
    b.from_fen("rnbqkbnr/pp1ppppp/2p5/8/8/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1");
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
    

    return 0;
}