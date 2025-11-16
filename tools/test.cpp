#include "roece.h"
#include "/usr/local/libcf/include/libcf.h"
#include <thread>
#include <condition_variable>

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

bool movepacked_comparitor(const void *lhs, const void *rhs, size_t keylen) {
    MovePacked& lhsr = *(MovePacked*)lhs;
    MovePacked& rhsr = *(MovePacked*)rhs;
    return lhsr.w == rhsr.w;
}

struct QueueEntry {
    PositionPacked pos;
    Move           mv;
    PositionPacked par;
};

struct Counts {
    mutable int maxmoves;
    mutable int minmoves;
    mutable int allmoves;
    mutable int movecnt;
    mutable int white_root_coll;
    mutable int white_dom_coll;
    mutable int black_root_coll;
    mutable int black_dom_coll;
    mutable int newroots;
    mutable int newdomain;
    Counts()
    : maxmoves{0}
    , minmoves{999}
    , allmoves{0}
    , movecnt{0}
    , white_root_coll{0}
    , white_dom_coll{0}
    , black_root_coll{0}
    , black_dom_coll{0}
    , newroots{0}
    , newdomain{0}
    {}
};

std::mutex m_closing;
bool closing = false;
std::condition_variable cv;

// 68 maximum moves, 36 minimum.
#pragma pack(1)
struct Edge {
    uint16_t cnt;
    struct edge {
        MovePacked mv;
        uint64_t   pos;
        edge(){};
    } edges[36];
    
    Edge() : cnt{0} {}
    int add(Move& m, uint64_t& pp) {
        edges[cnt].mv  = MovePacked(m);
        edges[cnt].pos = pp;
        return ++cnt;
    }
    void clear() {
        cnt = 0;
        memset(&edges, 0x00, sizeof(edge)*36);
    }
};
#pragma pack()

#define THREAD_CNT 1

Counts counts[THREAD_CNT];

const char *work_folder = "/tmp/phrogg";
const char *roots_folder = "/tmp/phrogg";

typedef std::shared_ptr<libcf::dht<PositionPacked>> DHTPtr;

struct roots {
    std::map<uint32_t, std::map<uint32_t, DHTPtr>> _m;
    roots() {
        _m[WHITE] = std::map<uint32_t,DHTPtr>();
        _m[BLACK] = std::map<uint32_t,DHTPtr>();
    }
    DHTPtr& get(const PositionPacked& pp) {
        auto  side{pp.gi.f.on_move};
        auto  piece_cnt{pp.gi.f.piece_cnt};
        auto& m = _m[side];
        if ( m.find(piece_cnt) == m.end() ) {
            std::stringstream ss;
            ss << side << std::setw(2) << std::setfill('0') << piece_cnt;
            m[piece_cnt] = std::make_shared<libcf::dht<PositionPacked>>(roots_folder, ss.str(), positionpacked_comparitor, positionpacked_hasher);
        }
        return m[piece_cnt];
    }
} all_roots;

// queue for positions in work
libcf::dq<QueueEntry> queue( work_folder, "queue", sizeof(PositionPacked) * 1024 );
// table for domain positions
DHTPtr pos[2] = {
    std::make_shared<libcf::dht<PositionPacked>>( work_folder, "white_pos", positionpacked_comparitor, positionpacked_hasher ),
    std::make_shared<libcf::dht<PositionPacked>>( work_folder, "black_pos", positionpacked_comparitor, positionpacked_hasher ) 
};

// libcf::dht<PositionPacked> white_pos( work_folder, "white_pos", positionpacked_comparitor, positionpacked_hasher ); 
// libcf::dht<PositionPacked> black_pos( work_folder, "black_pos", positionpacked_comparitor, positionpacked_hasher ); 
// table for root positions
// libcf::dht<PositionPacked> white_root( work_folder, "white_root", positionpacked_comparitor, positionpacked_hasher ); 
// libcf::dht<PositionPacked> black_root( work_folder, "black_root", positionpacked_comparitor, positionpacked_hasher ); 
// table for edges
libcf::dht<uint64_t,Edge> edges( work_folder, "edges", positionpacked_comparitor, positionpacked_hasher);

void thread_proc(int idx) {
    MoveList ml;
    Counts& cnts = counts[idx];
    QueueEntry qe;
    while ( !closing ) {
        std::unique_lock lk(m_closing);
        cv.wait(lk, []{ return !queue.empty() || closing; });
        lk.unlock();
        cv.notify_one();
        if ( closing )
            continue;
        
        if( queue.pop(qe) ) {
            PositionPacked pp{qe.pos};
            Board b{pp};
            uint64_t zob = b.zobristHash();
            Edge e;
            b.get_moves(ml);
            if (ml.size() > cnts.maxmoves)
                cnts.maxmoves = ml.size();
            if (ml.size() < cnts.minmoves)
                cnts.minmoves = ml.size();
            cnts.allmoves += ml.size();
            ++cnts.movecnt;
            // std::cout << b.fen() << std::endl;
            for ( auto mv : ml ) {
                // Board c(pp);
                MoveResult mr = b.apply_move(mv);
                PositionPacked p0 = b.pack();
                uint64_t zob_p0 = b.zobristHash();
                b.revert_move(mr);
                if ( e.add(mv, zob_p0) == 36 ) {
                    edges.insert(zob,e);
                    e.clear();
                }
                // std::cout << c.diagram() << std::endl << c.fen() << std::endl;
                if ( mv.resultsInRootPosition(mr.action) ) {
                    DHTPtr& roots = all_roots.get(p0);
                    if ( roots->search(p0) ) {
                        switch(b.get_on_move()) {
                            case WHITE: ++cnts.white_root_coll; break;
                            case BLACK: ++cnts.black_root_coll; break;
                        }
                        continue;
                    }
                    roots->insert(p0);
                    ++cnts.newroots;
                } else {
                    DHTPtr& posits = pos[b.get_on_move()];
                    if ( posits->search(p0) ) {
                        switch(b.get_on_move()) {
                            case WHITE: ++cnts.white_dom_coll; break;
                            case BLACK: ++cnts.black_dom_coll; break;
                        }
                        continue;
                    }
                    posits->insert(p0);
                    qe.pos = p0;
                    qe.mv  = mv;
                    qe.par = pp;
                    queue.push(qe);
                    ++cnts.newdomain;
                    cv.notify_one();
                }
            }
            if ( e.cnt ) {
                edges.insert(zob,e);
            }
        }
    }
}

int main() {
    std::cout << bi << std::endl << libcf::bi << std::endl;

    // Board b(false);
    // b.from_fen("1nrN4/2P5/8/8/4Pp2/8/2p5/1NRn4 w - e4 0 1");
    // Board b("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1");
    // Board b("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    Board b(INITIAL_POSITION);
    PositionPacked pp = b.pack();
    PositionPacked pi(pp);
    QueueEntry qe { pp, Move(), pp };
    queue.push(qe);

    std::cout << b.diagram() << std::endl << b.fen() << std::endl;

    if ( !pos[WHITE]->search(pp) )
        pos[WHITE]->insert(pp);

    std::vector<std::thread> threads;

    for (int i = 0; i < THREAD_CNT; ++i ) {
        threads.push_back(std::move(std::thread(thread_proc, i)));
    }

    cv.notify_one();

    do {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Counts totals;
        for (int i = 0; i < THREAD_CNT; ++i ) {
            Counts cnts = counts[i]; // copy
            std::cout        << cnts.maxmoves
                      << ' ' << cnts.minmoves
                      << ' ' << cnts.allmoves
                      << ' ' << cnts.movecnt
                      << ' ' << cnts.white_root_coll
                      << ' ' << cnts.white_dom_coll
                      << ' ' << cnts.black_root_coll
                      << ' ' << cnts.black_dom_coll
                      << ' ' << cnts.newroots
                      << ' ' << cnts.newdomain
                      << std::endl;
            if (totals.maxmoves < cnts.maxmoves) {
                totals.maxmoves = cnts.maxmoves;
            }
            if (totals.minmoves > cnts.minmoves) {
                totals.minmoves = cnts.minmoves;
            }
            totals.allmoves        += cnts.allmoves;
            totals.movecnt         += cnts.movecnt;
            totals.white_root_coll += cnts.white_root_coll;
            totals.white_dom_coll  += cnts.white_dom_coll;
            totals.black_root_coll += cnts.black_root_coll;
            totals.black_dom_coll  += cnts.black_dom_coll;
            totals.newroots        += cnts.newroots;
            totals.newdomain       += cnts.newdomain;
        }
        std::cout        << totals.maxmoves
                  << ' ' << totals.minmoves
                  << ' ' << totals.allmoves
                  << ' ' << totals.movecnt
                  << ' ' << totals.white_root_coll
                  << ' ' << totals.white_dom_coll
                  << ' ' << totals.black_root_coll
                  << ' ' << totals.black_dom_coll
                  << ' ' << totals.newroots
                  << ' ' << totals.newdomain
                  << ' ' << queue.size()
                  << std::endl
                  << std::endl;
        cv.notify_one();
    } while ( !queue.empty() );
    {
        std::lock_guard<std::mutex> _ (m_closing);
        closing = true;
        cv.notify_one();
    }
    // wait for all threads to complete
    for (std::thread& t : threads) {
        t.join();
    }

    return 0;
}
