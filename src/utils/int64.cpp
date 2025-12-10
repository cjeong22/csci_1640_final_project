#include "../../include/utils/int64.hpp"

int64 modq(int64 x, int64 q) {
    int64 r = x % q;
    if (r < 0) r += q;
    return r;
};