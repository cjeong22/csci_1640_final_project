#pragma once
#include <vector>
#include <cstddef>

#include "./int64.hpp"
#include "./rng.hpp"

using std::vector;

using Matrix = vector<vector<int64>>;
using Vector = vector<int64>;

Matrix zeros(int rows, int cols);
Matrix randomMatrixMod(int rows, int cols, int64 q, RNG &rng);
Vector randomVectorMod(int len, int64 q, RNG &rng);
Vector matVecMulMod(const Matrix &A, const Vector &x, int64 q);