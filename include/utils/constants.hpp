#pragma once 
#include <vector>
#include "./matrix.hpp"

namespace Constants {
    constexpr int m = 2;
    constexpr int n = 3;

    // ensure q is a power of 2
    constexpr int q = 8;

    // ensure log_2(q) = k
    constexpr int k = 3;

    inline const Matrix sk = [](){
        Matrix temp(n, 1);
        temp(0, 0) = 3;
        temp(1, 0) = 4;
        temp(2, 0) = 7;
        return temp;
    }();

    inline const Matrix pk = [](){
        Matrix temp(m, n);
        temp(0, 0) = 1; temp(0, 1) = 3; temp(0, 2) = 7;
        temp(1, 0) = 5; temp(1, 1) = 4; temp(1, 2) = 2;
        return temp;
    }();

    inline const Matrix e = []() {
        Matrix temp(m, 1);
        temp(0, 0) = 1;
        temp(1, 0) = -1;
        return temp;
    }();

    inline const Matrix gadget_vector = []() {
        Matrix temp(k, 1);
        temp(0, 0) = 1;
        temp(1, 0) = 2;
        temp(2, 0) = 4;
        return temp;
    }();

    inline const Matrix b = (Constants::pk).multiply(Constants::sk).add(Constants::e);
}