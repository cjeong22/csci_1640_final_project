#include <iostream>

#include "../../include/utils/matrix.hpp"
#include "../../include/utils/rng.hpp"
#include "../../include/gsw/params.hpp"

using std::cout;

Matrix zeros(int k) {
    return Matrix(k, vector<int64>(k, 0));
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

Matrix matMulMod(const Matrix &A, const Matrix &B, const Params &p) {
    Matrix C = zeros(p.k);
    for (int i = 0; i < p.k; ++i)
        for (int j = 0; j < p.k; ++j) {
            long long acc = 0;
            for (int t = 0; t < p.k; ++t)
                acc += A[i][t] * B[t][j];
            C[i][j] = modq(acc, p.q);
        }
    return C;
}


void printVector(const Vector &v) {
    for (auto x : v) cout << x << " ";
    cout << "\n";
}

void printMatrix(const Matrix &M) {
    for (const auto &row : M) {
        for (auto x : row) cout << x << " ";
        cout << "\n";
    }
}