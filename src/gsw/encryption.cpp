#include "../../include/gsw/encryption.hpp"
#include "../../include/gsw/params.hpp"
#include "../../include/utils/matrix.hpp"

using std::runtime_error;

Matrix gadgetMatrixG(const Params &p) {
    Matrix G = zeros(p.k);
    int64 val = 1;
    for (int i = 0; i < p.k; ++i) {
        if (i > 0) val = (val * p.B) % p.q;
        G[i][i] = val;
    }
    return G;
}

Matrix gadgetInverse(const Matrix &C, const Params &p, const Matrix &G) {
    Matrix T = zeros(p.k);
    for (int i = 0; i < p.k; ++i) {
        int64 gi = G[i][i];
        for (int j = 0; j < p.k; ++j) {
            int64 x = C[i][j];
            if (x > p.q / 2) x -= p.q;
            long double t = (long double)x / (long double)gi;
            T[i][j] = (int64)std::llround(t);
        }
    }
    return T;
}

Matrix encryptInt(int m, const Params &p, RNG &rng, const Matrix &G) {
    Matrix C = zeros(p.k);

    for (int i = 0; i < p.k; ++i)
        for (int j = 0; j < p.k; ++j)
            C[i][j] = modq((int64)m * G[i][j], p.q);

    for (int i = 0; i < p.k; ++i)
        for (int j = 0; j < p.k; ++j)
            C[i][j] = modq(C[i][j] + rng.smallError(), p.q);

    return C;
}
