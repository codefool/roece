#include <iostream>
#include <initializer_list>
#include <map>
#include <vector>

enum Direction {
    UP = 0,
    RGT,
    DN,
    LFT,
    UPR,
    DNR,
    DNL,
    UPL,
    KUL,
    KUR,
    KRU,
    KRD,
    KDR,
    KDL,
    KLD,
    KLU,
    DIR_CNT
};

const char *dirName[DIR_CNT] = {
    "UP",
    "RGT",
    "DN",
    "LFT",
    "UPR",
    "DNR",
    "DNL",
    "UPL",
    "KUL",
    "KUR",
    "KRU",
    "KRD",
    "KDR",
    "KDL",
    "KLD",
    "KLU"
};

struct Node {
    short rank;
    short file;
    Node *n[DIR_CNT];
    Node() {
        std::memset(n, 0x00, sizeof(n));
    }
    friend std::ostream& operator <<(std::ostream& os, Node *n) {
        os << '[' << n->rank << ',' << n->file << ']';
        return os;
    }    
};

class Board {
private:
    Node squares[8][8];

public:
    Board() 
    {
        auto setDir = [=](Node& n, Direction d, short r, short f) {
            if ( 0 <= r && r < 8 && 0 <= f && f < 8 )
                n.n[d] = &squares[r][f];
        };

        for (auto rank = 0; rank < 8; ++rank ) {
            for (auto file = 0; file < 8; ++file ) {
                Node& node{squares[rank][file]};
                node.rank = rank;
                node.file = file;
                // axial moves
                setDir(node, UP,  rank +1, file +0);
                setDir(node, RGT, rank +0, file +1);
                setDir(node, DN,  rank -1, file +0);
                setDir(node, LFT, rank +0, file -1);
                // diagonal
                setDir(node, UPR, rank +1, file +1);
                setDir(node, DNR, rank -1, file +1);
                setDir(node, DNL, rank -1, file -1);
                setDir(node, UPL, rank +1, file -1);
                // knights
                setDir(node, KUL, rank +2, file -1);
                setDir(node, KUR, rank +2, file +1);
                setDir(node, KRU, rank +1, file +2);
                setDir(node, KRD, rank -1, file +2);
                setDir(node, KDR, rank -2, file +1);
                setDir(node, KDL, rank -2, file -1);
                setDir(node, KLD, rank -1, file -2);
                setDir(node, KLU, rank +1, file -2);
            }
        }
    }

    void dump() {
        for (auto rank = 0; rank < 8; ++rank ) {
            for (auto file = 0; file < 8; ++file ) {
                Node &n = squares[rank][file];

                std::cout << &n << ':' << std::endl;
                for ( auto d = 0; d < DIR_CNT; ++d ) {
                    Node *np = n.n[d];
                    if ( np != nullptr ) {
                        std::cout << ' ' << dirName[d] << ' ' << np << std::endl;
                    }
                }
            }
        }
    }
};

int main() {
    Board b;
    b.dump();
    return 0;
}



