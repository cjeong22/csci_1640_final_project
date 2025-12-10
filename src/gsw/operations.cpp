#include "../../include/gsw/operations.hpp"
#include "../../include/gsw/params.hpp"
#include "../../include/utils/matrix.hpp"
#include "../../include/gsw/encryption.hpp"

Matrix addC(const Matrix &C1, const Matrix &C2, const Params &p) {
    Matrix C = zeros(p.k);
    for (int i = 0; i < p.k; ++i)
        for (int j = 0; j < p.k; ++j)
            C[i][j] = modq(C1[i][j] + C2[i][j], p.q);
    return C;
}

// -----------------------------
// Homomorphic multiplication
//
// C1 ≈ m1 * G
// C2 ≈ m2 * G
// T2 = G^{-1}(C2) ≈ m2 * I
// Cmul = C1 * T2 ≈ (m1 * m2) * G
// -----------------------------
Matrix mulC(const Matrix &C1, const Matrix &C2, const Params &p, const Matrix &G) {
    Matrix T2 = gadgetInverse(C2, p, G);
    return matMulMod(C1, T2, p);
}