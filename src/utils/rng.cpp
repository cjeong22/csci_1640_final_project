#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <stdexcept>
#include <cmath>

#include "../../include/utils/int64.hpp"
#include "../../include/utils/rng.hpp"


RNG::RNG(int64 seed)
    : gen(seed ? seed : std::random_device{}()),
      dist_uniform(0, std::numeric_limits<int64>::max())
{}

int64 RNG::uniformMod(int64 q) {
    return dist_uniform(gen) % q;
}

int RNG::bit() {
    return (int)(dist_uniform(gen) & 1);
}

int RNG::smallError() {
    int r = (int)(dist_uniform(gen) % 10);
    if (r == 0) return 1;
    if (r == 1) return -1;
    return 0;
}