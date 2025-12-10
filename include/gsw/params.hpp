#ifndef PARAMS_HPP
#define PARAMS_HPP

#include "../utils/int64.hpp"

struct Params {
    int k;      // matrix dimension
    int64 q;    // modulus
    int B;      // gadget base
};

#endif