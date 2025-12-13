#include "../../include/gsw/operations.hpp"
#include "../../include/gsw/params.hpp"
#include "../../include/utils/matrix.hpp"
#include "../../include/gsw/encryption.hpp"

Matrix addC(const Matrix &C1, const Matrix &C2, const Params &p) {
    Matrix C = Matrix(p.k);
    for (int i = 0; i < p.k; ++i)
        for (int j = 0; j < p.k; ++j)
            C(i, j) = modq(C1(i, j) + C2(i, j), p.q);
    return C;
}

Matrix mulC(const Matrix &C1, const Matrix &C2, const Params &p, const Matrix &G) {
    Matrix T2 = gadgetInverse(C2, p, G);
    return matMulMod(C1, T2, p);
}