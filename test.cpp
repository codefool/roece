#include "roece.h"
#include "../libcf/include/libcf.h"

bool positionpacked_comparitor(libcf::ucharptr_c lhs, libcf::ucharptr_c rhs, size_t keylen) {
    // lhs and rhs are pointers to PositionPacked structures - we only want to 
    // compare the population of the board and ignore the game information.
    PositionPacked& lhsr = *(PositionPacked*)lhs;
    PositionPacked& rhsr = *(PositionPacked*)rhs;
    return lhsr.pop == rhsr.pop && lhsr.hi == rhsr.hi && lhsr.lo == rhsr.lo;
    // return *(PositionPacked*)lhs == *(PositionPacked*)rhs;
}

// queue for positions in work
libcf::dq<PositionPacked> work( "/tmp", "positions" );
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
    work.push(pp);
    MoveList ml;
    while( work.pop(pp) ) {
        domain_positions.insert(pp);
        b.init(pp);
        b.get_moves(ml);
        int newroots(0);
        int newdomain(0);
        for ( auto mv : ml ) {
            Board c(pp);
            MoveAction ma = c.apply_move(mv);
            PositionPacked p0 = c.pack();
            if ( mv.resultsInRootPosition(ma) ) {
                if ( !root_positions.search(p0) ) {
                    root_positions.insert(p0);
                    newroots++;
                }
            } else {
                if ( !domain_positions.search(p0) ) {
                    // we haven't processed this position before so queue it
                    work.push(p0);
                } 
            }
        }
        std::cout << b.fen() << work.size() << '/' << newroots << '/' << newdomain << std::endl;
    }
    return 0;
}