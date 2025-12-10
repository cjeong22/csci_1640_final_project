#ifndef KEYS_HPP
#define KEYS_HPP

#include "../utils/matrix.hpp"
#include "./params.hpp"

struct SecretKey {
    Vector s;
};

struct PublicKey {
    Matrix A; 
    Vector u; 
};

void keygen(const Params &params, RNG &rng, SecretKey &sk, PublicKey &pk);

#endif