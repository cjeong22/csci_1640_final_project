#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <stdexcept>
#include <cmath>

#include "../../include/utils/rng.hpp"
#include "../../include/utils/int64.hpp"

RNG::RNG() : gen(std::random_device{}()), dist(0, std::numeric_limits<int64>::max()) {}

int RNG::randInt(int max_m) {  // uniform in [0, max_m]
    return (int)(dist(gen) % (max_m + 1));
}

int RNG::smallError() {
    int r = (int)(dist(gen) % 10);
    if (r == 0) return 1;
    if (r == 1) return -1;
    return 0;
}