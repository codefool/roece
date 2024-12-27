#include "roece.h"
#include "/usr/local/libcf/include/libcf.h"

bool positionpacked_comparitor(const void *lhs, const void *rhs, size_t keylen) {
    // lhs and rhs are pointers to PositionPacked structures - we only want to 
    // compare the population of the board and ignore the game information.
    PositionPacked& lhsr = *(PositionPacked*)lhs;
    PositionPacked& rhsr = *(PositionPacked*)rhs;
    bool ret = lhsr.compareNoClocks(rhsr);
    return ret;
    // return lhsr.pop == rhsr.pop && memcmp(lhsr.pieces.w, rhsr.pieces.w, sizeof(PositionPacked::pieces)) == 0;
}

// hasher that does not consider move clocks.
std::string positionpacked_hasher(const void *key, size_t keylen, size_t hashlen) {
    PositionPacked pp = *(PositionPacked*)key;
    pp.gi.i = pp.gi.packed_no_clocks();
    // pp.gi.f.full_move_cnt = pp.gi.f.half_move_clock = 0;

    libcf::MD5 md5;
    md5.update(&pp, sizeof(PositionPacked));
    md5.finalize();
    return md5.hexdigest().substr( 0, hashlen );
}

struct QueueEntry {
    PositionPacked pos;
    Move           mv;
    PositionPacked par;
};

// queue for positions in work
libcf::dq<QueueEntry> work( "/tmp/work", "positions", sizeof(PositionPacked) * 1024 );
// table for domain positions
libcf::dht<PositionPacked> domain_positions( "/tmp/work", "domain", positionpacked_comparitor, positionpacked_hasher ); 
// table for root positions
libcf::dht<PositionPacked> root_positions( "/tmp/work", "roots", positionpacked_comparitor, positionpacked_hasher ); 

int main() {
    std::cout << bi << std::endl << libcf::bi << std::endl;

    // Board b(false);
    // b.from_fen("1nrN4/2P5/8/8/4Pp2/8/2p5/1NRn4 w - e4 0 1");
    Board b(INITIAL_POSITION);
    PositionPacked pp = b.pack();
    PositionPacked pi(pp);
    QueueEntry qe { pp, Move(MV_NONE, Square::UNBOUNDED, Square::UNBOUNDED), pp };
    work.push(qe);

    std::cout << b.diagram() << std::endl << b.fen() << std::endl;

    if ( !root_positions.search(pp) )
        root_positions.insert(pp);
    // work.push(pp);
    MoveList ml;
    int maxmoves(0);
    int minmoves(100);
    int allmoves(0);
    int movecnt(0);

    std::cout << "pp.pieces size " << sizeof(PositionPacked::pieces) << std::endl;
    while( work.pop(qe) ) {
        pp = qe.pos;
        b.init(pp);
        b.zobristHash();
        b.get_moves(ml);
        if (ml.size() > maxmoves)
            maxmoves = ml.size();
        if (ml.size() < minmoves)
            minmoves = ml.size();
        allmoves += ml.size();
        movecnt++;
        std::cout << b.fen() << std::endl;
        int newroots(0);
        int newdomain(0);
        for ( auto mv : ml ) {
            Board c(pp);
            MoveAction ma = c.apply_move(mv);
            PositionPacked p0 = c.pack();

            // std::cout << c.diagram() << std::endl << c.fen() << std::endl;
            const char *res;
            if ( mv.resultsInRootPosition(ma) ) {
                if ( !root_positions.search(p0) ) {
                    root_positions.insert(p0);
                    newroots++;
                    res = "r+";
                } else {
                    continue;
                }
            } else {
                if ( !domain_positions.search(p0) ) {
                    // we haven't processed this position before so queue it
                    domain_positions.insert(p0);

                    qe.pos = p0;
                    qe.mv  = mv;
                    qe.par = pp;
                    work.push(qe);
                    newdomain++;
                    res = "d+";
                } else {
                    continue;
                }
            }
            if (res != nullptr) {
                std::cout << '\t' 
                        // << mv << ' '
                        << std::setfill(' ') << std::setw(64) << std::left << c.fen() << ' '
                        << positionpacked_hasher(&p0, sizeof(PositionPacked), 3) 
                        << ' ' << res 
                        << std::endl;
            }
        }
        std::cout << std::hex << std::setw(16) << std::setfill('0') << b.zobristHash() << ' ' << pp.hexString() 
                  << ':' << std::setfill(' ') << std::setw(64) << std::left << b.fen() << ':' 
                  << std::dec << std::setw(8) << std::right << work.size() 
                  << '/' << std::setw(8) << std::right << newroots 
                  << '/' << std::setw(8) << std::right << newdomain 
                  << std::endl;
    }
    std::cout << "Max moves: " << maxmoves << " Min moves:" << minmoves << " Avg moves:" << (allmoves / movecnt) << std::endl;
    return 0;
}