#pragma once
#include <vector>
#include <cstddef>

class Matrix {
    public:
        Matrix(size_t rows, size_t cols);
        int &operator()(size_t r, size_t c);
        const int &operator()(size_t r, size_t c) const;
        friend std::ostream& operator<<(std::ostream& os, const Matrix& m); 

        Matrix multiply(const Matrix& m) const;
        Matrix add(const Matrix &m) const;

        size_t rows() const;
        size_t cols() const;

    private:
        size_t rows_;
        size_t cols_;
        std::vector<int> data_;
};