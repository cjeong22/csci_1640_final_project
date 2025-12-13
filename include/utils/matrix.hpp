#pragma once
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <cstddef>

#include "./int64.hpp"
#include "./rng.hpp"
#include "../gsw/params.hpp"

using std::vector;

using Vector = vector<int64>;

struct Matrix {
    int k;
    vector<int64> a;

    explicit Matrix(int k);

    inline int64& operator()(int i,int j){ return a[(size_t)i*k + j]; }
    inline int64  operator()(int i,int j) const { return a[(size_t)i*k + j]; }

};

Matrix zeros(int k);
Vector matVecMulMod(const Matrix &A, const Vector &x, int64 q);
Matrix matMulMod(const Matrix &A, const Matrix &B, const Params &p);

#endif
