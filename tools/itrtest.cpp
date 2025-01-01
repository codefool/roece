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
    int maxmoves;
    int minmoves;
    int allmoves;
    int movecnt;
    int root_coll;
    int dom_coll;
    int newroots;
    int newdomain;
    Counts()
    : maxmoves{0}
    , minmoves{999}
    , allmoves{0}
    , movecnt{0}
    , root_coll{0}
    , dom_coll{0}
    , newroots{0}
    , newdomain{0}
    {}
};

std::mutex m_closing;
bool closing = false;
std::condition_variable cv;

// 68 maximum moves, 36 minimum.
struct Edge {
    uint16_t cnt;
    struct edge {
        MovePacked     mv;
        PositionPacked pos;
        edge(){};
    } edges[36];
    
    Edge() : cnt{0} {}
    int add(Move& m, PositionPacked& pp) {
        edges[cnt].mv  = MovePacked(m);
        edges[cnt].pos = pp;
        return ++cnt;
    }
    void clear() {
        cnt = 0;
        memset(&edges, 0x00, sizeof(edge)*36);
    }
    friend std::ostream& operator<<(std::ostream& os, const Edge& e);
};

std::ostream& operator<<(std::ostream& os, const Edge& e) {
    os << e.cnt;
    return os;
}
#pragma pack(1)
struct Edge0 {
    uint16_t cnt;
    struct edge {
        MovePacked     mv;
        uint64_t      pos;
        edge(){};
    } edges[36];
    
    Edge0() : cnt{0} {}
    int add(const MovePacked& m, const uint64_t& pp) {
        edges[cnt].mv  = m;
        edges[cnt].pos = pp;
        return ++cnt;
    }
    void clear() {
        cnt = 0;
        memset(&edges, 0x00, sizeof(edge)*36);
    }
};
#pragma pack()



#define THREAD_CNT 8

Counts counts[THREAD_CNT];

const char *work_folder = "/tmp/work0";

// table for domain positions
libcf::dht<PositionPacked,Edge> edges( work_folder, "edges", positionpacked_comparitor, positionpacked_hasher);
libcf::dht<uint64_t,Edge0> edges0( work_folder, "edges0", positionpacked_comparitor, positionpacked_hasher);

int main() {
    uint64_t cnt{0};
    for ( auto itr = edges.begin(); itr != edges.end(); ++itr) {
        auto edge = *itr;
        uint64_t zobSrc = Board(edge.first).zobristHash();
        Edge0 e0;
        for(int i{0}; i < edge.second.cnt; ++i) {
            uint64_t zobEdg = Board(edge.second.edges[i].pos).zobristHash();
            e0.add(edge.second.edges[i].mv, zobEdg);
        }
        edges0.insert(zobSrc,e0);
    }
    return 0;
}
