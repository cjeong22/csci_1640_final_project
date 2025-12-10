#ifndef RNG_HPP
#define RNG_HPP

#include <random>
#include <limits>
#include "../../include/utils/int64.hpp"

struct RNG {
    std::mt19937_64 gen;
    std::uniform_int_distribution<int64> dist;
    explicit RNG();

    int randInt(int max_m);
    int smallError();
};

#endif