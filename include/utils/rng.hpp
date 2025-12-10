#ifndef RNG_HPP
#define RNG_HPP

#include <random>
#include <limits>
#include "../../include/utils/int64.hpp"

struct RNG {
    std::mt19937_64 gen;
    std::uniform_int_distribution<int64> dist_uniform;

    explicit RNG(int64 seed = 0);

    int64 uniformMod(int64 q);
    int bit();
    int smallError();
};

#endif