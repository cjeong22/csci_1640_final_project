#include "../../include/gsw/encryption.hpp"
#include "../../include/gsw/params.hpp"
#include "../../include/utils/matrix.hpp"

using std::runtime_error;

Matrix gadgetMatrixG(const Params &params) {
    Matrix G = zeros(params.ell, params.n + 1);
    for (int i = 0; i < params.ell; ++i) {
        int64 val = 1;
        for (int j = 0; j < i; ++j) {
            val = (val * params.B) % params.q;
        }
        G[i][params.n] = val;
    }
    return G;
}

Matrix encryptBit(int m, const PublicKey &pk, const Params &params, RNG &rng) {
    if (m != 0 && m != 1) {
        throw runtime_error("encryptBit expects m in {0,1}");
    }

    Matrix Ahat(params.m, Vector(params.n + 1));
    for (int i = 0; i < params.m; ++i) {
        for (int j = 0; j < params.n; ++j) {
            Ahat[i][j] = pk.A[i][j];
        }
        Ahat[i][params.n] = pk.u[i];
    }

    Matrix G = gadgetMatrixG(params);
    Matrix C = zeros(params.ell, params.n + 1);

    int64 T = params.q / 8;

    for (int row = 0; row < params.ell; ++row) {
        vector<int> Xcol(params.m);
        for (int i = 0; i < params.m; ++i)
            Xcol[i] = rng.bit();

        for (int i = 0; i < params.m; ++i) {
            if (!Xcol[i]) continue;
            for (int j = 0; j < params.n + 1; ++j)
                C[row][j] += Ahat[i][j];
        }

        if (m == 1) {
            for (int j = 0; j < params.n + 1; ++j) {
                int64 add = (T * G[row][j]) % params.q;
                C[row][j] += add;
            }
        }

        for (int j = 0; j < params.n + 1; ++j)
            C[row][j] = modq(C[row][j], params.q);
    }

    return C;
}