#include "../../include/utils/matrix.hpp"
#include "../../include/utils/rng.hpp"

Matrix zeros(int rows, int cols) {
    return Matrix(rows, Vector(cols, 0));
}

Matrix randomMatrixMod(int rows, int cols, int64 q, RNG &rng) {
    Matrix M(rows, Vector(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            M[i][j] = rng.uniformMod(q);
    return M;
}

Vector randomVectorMod(int len, int64 q, RNG &rng) {
    Vector v(len);
    for (int i = 0; i < len; ++i)
        v[i] = rng.uniformMod(q);
    return v;
}

Vector matVecMulMod(const Matrix &A, const Vector &x, int64 q) {
    int rows = (int)A.size();
    int cols = (int)A[0].size();
    Vector res(rows, 0);
    for (int i = 0; i < rows; ++i) {
        long long acc = 0;
        for (int j = 0; j < cols; ++j)
            acc += A[i][j] * x[j];
        res[i] = modq(acc, q);
    }
    return res;
}