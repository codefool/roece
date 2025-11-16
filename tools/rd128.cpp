// rnd128 - generate 128-bit random numbers
//
// https://www.chessprogramming.org/Zobrist_Hashing

#include <iostream>
#include <iomanip>
#include <array>
#include <bitset>
#include <cstdint>
#include <random>
#include <vector>

// hacker's delight 2nd ed Fig 5-1
// int bitpop64(std::uint64_t x) {
//     x = ( x & 0x5555555555555555ULL) + ((x >>  1) & 0x5555555555555555ULL);
//     x = ( x & 0x3333333333333333ULL) + ((x >>  2) & 0x3333333333333333ULL);
//     x = ( x & 0x0f0f0f0f0f0f0f0fULL) + ((x >>  4) & 0x0f0f0f0f0f0f0f0fULL);
//     x = ( x & 0x00ff00ff00ff00ffULL) + ((x >>  8) & 0x00ff00ff00ff00ffULL);
//     x = ( x & 0x0000ffff0000ffffULL) + ((x >> 16) & 0x0000ffff0000ffffULL);
//     x = ( x & 0x00000000ffffffffULL) + ((x >> 32) & 0x00000000ffffffffULL);
//     return (int)x;
// }
// hacker's delight (2nd ed) Fig 5-2
int bitpop64(std::uint64_t x) {
    x =  x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >>  4)) & 0x0f0f0f0f0f0f0f0fULL;
    x =  x + (x >>  8);
    x =  x + (x >> 16);
    x =  x + (x >> 32);
    return (int)(x & 0x000000000000003FULL);
}

int bitpop32(std::uint32_t x) {
    x =  x - ((x >> 1) & 0x55555555U);
    x = (x & 0x33333333U) + ((x >> 2) & 0x33333333U);
    x = (x + (x >>  4)) & 0x0f0f0f0fU;
    x =  x + (x >>  8);
    x =  x + (x >> 16);
    return (int)(x & 0x0000003FU);
}

// int bitpop32(std::uint32_t x) {
//     x = ( x & 0x55555555U) + ((x >>  1) & 0x55555555U);
//     x = ( x & 0x33333333U) + ((x >>  2) & 0x33333333U);
//     x = ( x & 0x0f0f0f0fU) + ((x >>  4) & 0x0f0f0f0fU);
//     x = ( x & 0x00ff00ffU) + ((x >>  8) & 0x00ff00ffU);
//     x = ( x & 0x0000ffffU) + ((x >> 16) & 0x0000ffffU);
//     return (int)x;
// }

int hamdist64(std::uint64_t lhs, std::uint64_t rhs) {
    auto ham = lhs ^ rhs;
    return bitpop64(ham);
}

int hamdist128(std::uint64_t *lhs, std::uint64_t *rhs) {
    return hamdist64(lhs[0], rhs[0]) + hamdist64(lhs[1], rhs[1]);
}

bool checkHamDistance(std::vector<std::uint64_t>& rd, std::uint64_t c, int minHam) {
    for(auto x:rd) {
        if (hamdist64(x,c) < minHam)
            return false;
    }
    return true;
}

template <size_t N>
void increment( std::bitset<N>& in ) {
    //  add 1 to each value, and if it was 1 already, carry the 1 to the next.
    for ( size_t i = 0; i < N; ++i ) {
        if ( in[i] == 0 ) {  // There will be no carry
            in[i] = 1;
            break;
        }
        in[i] = 0;  // This entry was 1; set to zero and carry the 1
    }
    return in;
}

template <size_t N>
size_t whichbits( std::bitset<N>& in, std::vector<size_t>& set ) {
    set.clear();
    for ( size_t i = 0; i < N; ++i ) {
        if ( in[i] == 1 )
            set.push_back(i);
    }
    return set.size();
}

int main() {
    std::random_device rd{};
    std::seed_seq ss{ rd(), rd(), rd() };
    std::mt19937_64 gen_{ ss };

    std::uniform_int_distribution<std::uint64_t> dis_{};
    std::vector<std::uint64_t> rn;

    // how many random numbers do we need?
    // 64 squares x 12 piece types (black+white)[768]
    //  1 black is on move
    //  4 castle rights
    //  8 en-passant
    const int needs = (64*12) + 1 + 4 + 8;

    rn.push_back(dis_(gen_));
    while (rn.size() < needs) {
        auto c = dis_(gen_);
        if (checkHamDistance(rn,c,19)) {
            rn.push_back(c);
        }
    }

    // exhaustive test to assure no combination of rn results in zero
    std::bitset<needs> n;
    std::vector<size_t> bits;
    while( !n.all() ) {
        increment(n);
        if ( whichbits(n, bits) ) {
            std::uint64_t mask{0};
            for ( auto bit:bits ) 
                mask ^= rn[bit];
            if (mask == 0) {
                std::cout << "collision " << std::endl;
                for ( auto bit:bits ) {
                    std::cout << bit << ' ' << n[bit] << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }

    int cnt{0};
    for (auto x:rn) {
        if (cnt % 8 == 0) {
            std::cout << std::endl;
        }
        if (cnt % 64 == 0) {
            std::cout << std::endl;
        }
        std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0') << x << ", ";
        ++cnt;
    }
    std::cout << std::endl;

    return 0;
}