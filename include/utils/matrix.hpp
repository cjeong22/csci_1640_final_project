#pragma once
#include <vector>
#include <cstddef>

#include "./int64.hpp"
#include "./rng.hpp"
#include "../gsw/params.hpp"

using std::vector;

using Matrix = vector<vector<int64>>;
using Vector = vector<int64>;

Matrix zeros(int k);
Vector matVecMulMod(const Matrix &A, const Vector &x, int64 q);
Matrix matMulMod(const Matrix &A, const Matrix &B, const Params &p);
void printVector(const Vector &v);
void printMatrix(const Matrix &M);