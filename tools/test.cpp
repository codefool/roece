#include "roece.h"
#include "../libcf/include/libcf.h"

bool positionpacked_comparitor(libcf::ucharptr_c lhs, libcf::ucharptr_c rhs, size_t keylen) {
    // lhs and rhs are pointers to PositionPacked structures - we only want to 
    // compare the population of the board and ignore the game information.
    PositionPacked& lhsr = *(PositionPacked*)lhs;
    PositionPacked& rhsr = *(PositionPacked*)rhs;
    // return lhsr == rhsr;
    return lhsr.pop == rhsr.pop && !memcmp(lhsr.pieces.w, rhsr.pieces.w, sizeof(PositionPacked::pieces));
}

// queue for positions in work
libcf::dq<PositionPacked> work( "/tmp", "positions", sizeof(PositionPacked) * 1024 );
// table for domain positions
libcf::dht<PositionPacked> domain_positions( "/tmp", "domain", positionpacked_comparitor ); 
// table for root positions
libcf::dht<PositionPacked> root_positions( "/tmp", "roots", positionpacked_comparitor ); 

int main() {
    std::cout << bi << std::endl;
    // Board b(false);
    // b.from_fen("1nrN4/2P5/8/8/4Pp2/8/2p5/1NRn4 w - e4 0 1");
    Board b(INITIAL_POSITION);
    PositionPacked pp = b.pack();
    if ( !root_positions.search(pp) )
        root_positions.insert(pp);
    work.push(pp);
    MoveList ml;
    int maxmoves(0);
    int minmoves(100);
    int allmoves(0);
    int movecnt(0);
    while( work.pop(pp) ) {
        std::cout << "->" << pp.hexString() << std::endl;
        domain_positions.insert(pp);
        b.init(pp);
        b.zobristHash();
        b.get_moves(ml);
        std::cout << b.diagram() << std::endl;
        if (ml.size() > maxmoves)
            maxmoves = ml.size();
        if (ml.size() < minmoves)
            minmoves = ml.size();
        allmoves += ml.size();
        movecnt++;
        int newroots(0);
        int newdomain(0);
        for ( auto mv : ml ) {
            Board c(pp);
            MoveAction ma = c.apply_move(mv);
            PositionPacked p0 = c.pack();

            std::cout << mv << std::endl 
                      << "Before:" << b.diagram()
                      << b.pack().hexString() << std::endl
                      << "After:" << c.diagram() 
                      << p0.hexString()
                      << std::endl;

            if ( mv.resultsInRootPosition(ma) ) {
                if ( !root_positions.search(p0) ) {
                    root_positions.insert(p0);
                    newroots++;
                }
            } else {
                if ( !domain_positions.search(p0) ) {
                    // we haven't processed this position before so queue it
                    work.push(p0);
                    std::cout << "<-" << p0.hexString() << c.diagram() << std::endl;
                } 
            }
        }
        std::cout << std::hex << std::setw(16) << std::setfill('0') << b.zobristHash() << ' ' << pp.hexString() << ':' << b.fen() << ':' << std::dec << work.size() << '/' << newroots << '/' << newdomain << std::endl;
    }
    std::cout << "Max moves: " << maxmoves << " Min moves:" << minmoves << " Avg moves:" << (allmoves / movecnt) << std::endl;
    return 0;
}