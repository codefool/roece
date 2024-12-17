// rnd128 - generate 128-bit random numbers
//
// 

#include <iostream>
#include <iomanip>
#include <array>
#include <cstdint>
#include <random>
#include <vector>

int bitpop64(std::uint64_t x) {
    x = ( x & 0x5555555555555555ULL) + ((x >>  1) & 0x5555555555555555ULL);
    x = ( x & 0x3333333333333333ULL) + ((x >>  2) & 0x3333333333333333ULL);
    x = ( x & 0x0f0f0f0f0f0f0f0fULL) + ((x >>  4) & 0x0f0f0f0f0f0f0f0fULL);
    x = ( x & 0x00ff00ff00ff00ffULL) + ((x >>  8) & 0x00ff00ff00ff00ffULL);
    x = ( x & 0x0000ffff0000ffffULL) + ((x >> 16) & 0x0000ffff0000ffffULL);
    x = ( x & 0x00000000ffffffffULL) + ((x >> 32) & 0x00000000ffffffffULL);
    return (int)x;
}

int bitpop32(std::uint32_t x) {
    x = ( x & 0x55555555ULL) + ((x >>  1) & 0x55555555ULL);
    x = ( x & 0x33333333ULL) + ((x >>  2) & 0x33333333ULL);
    x = ( x & 0x0f0f0f0fULL) + ((x >>  4) & 0x0f0f0f0fULL);
    x = ( x & 0x00ff00ffULL) + ((x >>  8) & 0x00ff00ffULL);
    x = ( x & 0x0000ffffULL) + ((x >> 16) & 0x0000ffffULL);
    return (int)x;
}

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

int main() {
    std::random_device rd{};
    std::seed_seq ss{ rd(), rd(), rd() };
    std::mt19937_64 gen_{ ss };

    std::uniform_int_distribution<std::uint64_t> dis_{};
    std::vector<std::uint64_t> rn;

    int needs = (64*12) + 1 + 16 + 8;

    rn.push_back(dis_(gen_));
    while (rn.size() < needs) {
        auto c = dis_(gen_);
        if (checkHamDistance(rn,c,1)) {
            rn.push_back(c);
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