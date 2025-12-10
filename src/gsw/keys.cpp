#include "../../include/gsw/keys.hpp"

void keygen(const Params &params, RNG &rng, SecretKey &sk, PublicKey &pk) {
    sk.s = randomVectorMod(params.n, params.q, rng);
    pk.A = randomMatrixMod(params.m, params.n, params.q, rng);

    Vector e(params.m);
    for (int i = 0; i < params.m; ++i)
        e[i] = rng.smallError();

    Vector As = matVecMulMod(pk.A, sk.s, params.q);
    pk.u.resize(params.m);
    for (int i = 0; i < params.m; ++i) {
        pk.u[i] = modq(As[i] + e[i], params.q);
    }
}