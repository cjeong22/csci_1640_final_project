#include <cstddef>
#include <vector>
#include <iostream>

#include "../../include/utils/constants.hpp"
#include "../../include/utils/matrix.hpp"

Matrix::Matrix(size_t rows, size_t cols): rows_(rows), cols_(cols), data_(rows*cols) {};

int &Matrix::operator()(size_t r, size_t c) {
    return data_[r * cols_ + c];
}

const int &Matrix::operator()(size_t r, size_t c) const {
    return data_[r * cols_ + c];
}

Matrix Matrix::multiply(const Matrix& m) const{
    if (cols_ != m.rows_) {
        throw std::runtime_error("Matrix dimensions incorrect");
    }

    Matrix result(rows_, m.cols_);

    for (size_t row = 0; row < rows_; row++) {
        for (size_t col = 0; col < m.cols_; col++) {
            int sum = 0;
            for (size_t k = 0; k < cols_; k++) {
                sum += (*this)(row, k) * m(k, col);
                sum %= Constants::q;
            }
            result(row, col) = sum;
        }
    }

    return result;
}

Matrix Matrix::add(const Matrix &m) const {
    if (cols_ != m.cols_ || rows_ != m.rows_) {
        throw std::runtime_error("Matrix dimensions incorrect");
    }

    Matrix result(rows_, cols_);

    for (size_t row = 0; row < rows_; row++) {
        for (size_t col = 0; col < cols_; col++) {
            result(row, col) = (*this)(row, col) + m(row, col);
            result(row, col) %= Constants::q;
        }
    }

    return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    for (std::size_t r = 0; r < m.rows_; ++r) {
        os << "[ ";
        for (std::size_t c = 0; c < m.cols_; ++c) {
            os << m.data_[r * m.cols_ + c] << " ";
        }
        os << "]\n";
    }
    return os;
}

size_t Matrix::rows() const {
    return rows_;
}

size_t Matrix::cols() const{
    return cols_;
}
