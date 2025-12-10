#include "../../include/gsw/operations.hpp"
#include "../../include/gsw/params.hpp"
#include "../../include/utils/matrix.hpp"

Matrix addCiphertexts(const Matrix &C1, const Matrix &C2, const Params &params) {
    int rows = (int)C1.size();
    int cols = (int)C1[0].size();
    Matrix Csum = zeros(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Csum[i][j] = modq(C1[i][j] + C2[i][j], params.q);
        }
    }
    return Csum;
}